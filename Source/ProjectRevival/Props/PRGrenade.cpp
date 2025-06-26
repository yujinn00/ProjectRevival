// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/PRGrenade.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "Physics/PRCollision.h"

// Sets default values
APRGrenade::APRGrenade()
{
	// 충돌체 컴포넌트 설정.
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetRelativeScale3D(FVector(0.2f));
	RootComponent = Sphere;

	// 메시 컴포넌트 설정.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	// 투사체 이동 컴포넌트 설정.
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->InitialSpeed = 1500.0f;
	ProjectileMovement->MaxSpeed = 1500.0f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.4f;
	ProjectileMovement->Friction = 0.6f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 5.0f;

	// 스태틱 메시 할당 (크기: 0.2배).
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRef(TEXT("/Game/ParagonGideon/FX/Meshes/Heroes/Gideon/Abilities/SM_Meteor_Chunk_Small.SM_Meteor_Chunk_Small"));
	if (StaticMeshRef.Succeeded())
	{
		StaticMesh->SetStaticMesh(StaticMeshRef.Object);
	}

	// 파티클 이펙트 할당.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemRef(TEXT("/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Meteor/FX/P_Gideon_MeteorBigImpact.P_Gideon_MeteorBigImpact"));
	if (ParticleSystemRef.Succeeded())
	{
		ParticleSystem = ParticleSystemRef.Object;
	}

	// 카메라 셰이크 클래스 할당.
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeBaseRef(TEXT("/Game/ProjectRevival/Props/BP_GrenadeShake.BP_GrenadeShake_C"));
	if (CameraShakeBaseRef.Succeeded())
	{
		CameraShakeBase = CameraShakeBaseRef.Class;
	}
}

// Called when the game starts or when spawned
void APRGrenade::BeginPlay()
{
	Super::BeginPlay();

	// FuseTime 이후에 Explode() 함수를 호출하는 타이머 설정.
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &APRGrenade::Explode, FuseTime, false);
}

void APRGrenade::Explode()
{
	// 파티클 이펙트 재생 (크기: 1.5배).
	if (ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, GetActorLocation(), FRotator::ZeroRotator, FVector(1.5f));
	}

	// 카메라 셰이크 재생.
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PC->PlayerCameraManager && CameraShakeBase)
	{
		PC->PlayerCameraManager->StartCameraShake(CameraShakeBase, 1.0f);
	}

	// 데미지를 무시할 액터 목록 생성 (플레이어와 수류탄 추가).
	TArray<AActor*> ActorsToIgnore;
	if (AActor* GrenadeInstigator = GetInstigator())
	{
		ActorsToIgnore.Add(GrenadeInstigator);
	}
	ActorsToIgnore.Add(this);

	// 폭발 범위 내 모든 대상 탐색. 
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bIsHit = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		CCHANNEL_PRPLAYERACTION,
		FCollisionShape::MakeSphere(OuterRadius),
		Params
	);

	// 감지된 대상들에게 데미지 처리.
	if (bIsHit)
	{
		FDamageEvent DamageEvent;
		for (const auto& Overlap : Overlaps)
		{
			AActor* HitActor = Overlap.GetActor();
			if (HitActor && !ActorsToIgnore.Contains(HitActor))
			{
				// 거리 비례 데미지 계산.
				const float DistanceToTarget = (HitActor->GetActorLocation() - GetActorLocation()).Size();
				const float DamageRatio = FMath::Clamp(1.0f - (DistanceToTarget - InnerRadius) / (OuterRadius - InnerRadius), 0.0f, 1.0f);
				const float FinalDamage = FMath::Lerp(MinimumDamage, BaseDamage, DamageRatio);

				// 데미지 적용.
				HitActor->TakeDamage(FinalDamage, DamageEvent, GetInstigatorController(), this);
			}
		}
	}

	// 디버그 메시지 출력.
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("수류탄 투척"));

	// 디버그용 판정 범위 시각화.
#if ENABLE_DRAW_DEBUG
	const FColor OuterColor = FColor::Magenta;
	const FColor InnerColor = FColor::Red;
	const float DebugDuration = 1.0f;

	// 최대 폭발 범위.
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		OuterRadius,
		32,
		OuterColor,
		false,
		DebugDuration,
		0,
		1.0f
	);

	// 최대 데미지 범위.
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		InnerRadius,
		32,
		InnerColor,
		false,
		DebugDuration,
		0,
		1.0f
	);
#endif

	// 액터 파괴.
	Destroy();
}
