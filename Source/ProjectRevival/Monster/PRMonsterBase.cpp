// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/PRMonsterBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/PRCollision.h"
#include "AI/PRAIController.h"
#include "CharacterStat/PRCharacterStatComponent.h"
#include "UI/PRWidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "UI/PRHpBarWidget.h"
#include "Game/PRMainGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APRMonsterBase::APRMonsterBase()
{
	// 컴포넌트 설정.
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PRMONSTERCOLLISION);

	// 무브먼트 설정.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;

	// Mesh.
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// AIController 클래스 설정.
	AIControllerClass = APRAIController::StaticClass();

	// 빙의 모드 설정.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 몬스터 공격 몽타주 에셋 설정.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MonsterAttackActionMontageRef(TEXT("/Game/ProjectRevival/Animation/Monster/Wolf/AM_WolfAttack.AM_WolfAttack"));
	if (MonsterAttackActionMontageRef.Object)
	{
		MonsterAttackActionMontage = MonsterAttackActionMontageRef.Object;
	}

	// 몬스터 죽음 몽타주 에셋 설정.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MonsterDeadMontageRef(TEXT("/Game/ProjectRevival/Animation/Monster/Wolf/AM_WolfDead.AM_WolfDead"));
	if (MonsterDeadMontageRef.Object)
	{
		MonsterDeadMontage = MonsterDeadMontageRef.Object;
	}

	// Stat Component.
	Stat = CreateDefaultSubobject<UPRCharacterStatComponent>(TEXT("Stat"));
 
	// Widget Component.
	HpBar = CreateDefaultSubobject<UPRWidgetComponent>(TEXT("Widget"));

	// 컴포넌트 계층 설정 및 상대 위치 설정(머리 위로 보일 수 있게).
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));

	// 사용할 위젯 클래스 정보 설정.
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ProjectRevival/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		// 위젯 컴포넌트는 위젯의 클래스 정보를 바탕으로 자체적으로 인스턴스를 생성함.
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
 		
		// 2D 모드로 그리기.
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
 
		// 크기 설정.
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
 
		// 콜리전 끄기.
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APRMonsterBase::SetUpCharacterWidget(class UUserWidget* InUserWidget)
{
	// 필요한 위젯 정보 가져오기.
	UPRHpBarWidget* HpBarWidget = Cast<UPRHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		// 최대 체력 값 설정.
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());

		// HP 퍼센트가 제대로 계산되도록 현재 체력 설정.
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());

		// 체력 변경 이벤트(델리게이트)에 함수 및 객체 정보 등록.
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UPRHpBarWidget::UpdateHpBar);
	}
}

void APRMonsterBase::AttackHitCheck()
{
	// 공격 거리.
	const float AttackRange = 30.0f;

	// 충돌 시작 지점 계산.
	// 캐릭터 몸통에서 약간 앞으로(캡슐의 반지름만큼) 설정.
	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0f;
	FVector End = Start + GetActorForwardVector() * AttackRange;

	// SCENE_QUERY_STAT: 언리얼에서 지원하는 분석 툴에 태그를 추가하는 매크로.
	// 두 번째 인자: 복잡한 형태의 충돌체를 감지할지 여부.
	// 세 번째 인자: 무시할 액터 목록.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack),	false,	this);

	// 트레이스에 사용할 구체의 반지름.
	const float AttackRadius = 30.0f;

	// 트레이스를 활용해 충돌 검사.
	FHitResult OutHitResult;
	bool HitDetected = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		CCHANNEL_PRMONSTERACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// 충돌 감지된 경우의 처리.
	if (HitDetected)
	{
		// 데미지 수치.
		const float AttackDamage = 30.0f;

		// 데미지 이벤트.
		FDamageEvent DamageEvent;

		// 데미지 전달.
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

	// 충돌 디버그 (시각적으로 확인할 수 있도록).
#if ENABLE_DRAW_DEBUG
	// 캡슐의 중심 위치.
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;

	//캡슐 높이 절반 값.
	float CapsuleHalfHeight = AttackRange * 0.5f;

	// 표시할 색상 (안 맞으면 초록, 맞으면 빨강).
	FColor DrawColor = HitDetected ? FColor::Red : FColor::Green;

	// 캡슐 그리기.
	DrawDebugCapsule(
		GetWorld(),
		CapsuleOrigin,
		CapsuleHalfHeight,
		AttackRadius,
		FRotationMatrix::MakeFromX(GetActorForwardVector()).ToQuat(),
		DrawColor,
		false,
		5.0f
	);
#endif
}

float APRMonsterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 스탯 정보가 업데이트 되도록 데미지 전달.
	if (Stat)
	{
		Stat->ApplyDamage(DamageAmount);
	}

	// 0.1초의 히트 스톱 설정.
	CustomTimeDilation = 0.1f;
	GetWorldTimerManager().SetTimer(
		HitStopTimerHandle,
		this,
		&APRMonsterBase::EndHitStop,
		HitStopDuration,
		false
	);

	return DamageAmount;
}

void APRMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 죽었을 때 발행되는 이벤트에 SetDead 함수 등록.
	Stat->OnHpZero.AddUObject(this, &APRMonsterBase::SetDead);
}

float APRMonsterBase::GetAIPatrolRadius()
{
	return 600.0f;
}

float APRMonsterBase::GetAIDetectRange()
{
	return 600.0f;
}

float APRMonsterBase::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + GetAIAttackRadius() * 2.0f;
}

float APRMonsterBase::GetAIAttackRadius()
{
	return 50.0f;
}

float APRMonsterBase::GetAITurnSpeed()
{
	return 2.0f;
}

void APRMonsterBase::SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void APRMonsterBase::AttackByAI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		const float AttackSpeedRate = 1.0f;
		AnimInstance->Montage_Play(MonsterAttackActionMontage, AttackSpeedRate);

		// 몽타주 재생이 시작되면, 재생이 종료될 때 호출되는 델리게이트에 등록.
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APRMonsterBase::NotifyAttackEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MonsterAttackActionMontage);
	}
}

void APRMonsterBase::NotifyAttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// 전달 받은 델리게이트 실행.
	OnAttackFinished.ExecuteIfBound();
}

void APRMonsterBase::EndHitStop()
{
	// 몬스터의 시간 흐름을 원래 속도로 되돌림.
	CustomTimeDilation = 1.0f;
}

void APRMonsterBase::SetDead()
{
	APRMainGameMode* GameMode = Cast<APRMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		// 몬스터의 사망을 알림.
		GameMode->MonsterKilled();
	}

	// 무브먼트 컴포넌트 끄기.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 콜리전 끄기.
	SetActorEnableCollision(false);

	// 죽는 애니메이션 재생.
	PlayDeadAnimation();

	// 죽었을 때 HpBar 사라지도록 처리.
	if (HpBar)
	{
		HpBar->SetHiddenInGame(true);
	}
}

void APRMonsterBase::PlayDeadAnimation()
{
	// 몽타주 재생.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 이미 재생 중인 몽타주가 있다면, 모두 종료.
		AnimInstance->StopAllMontages(0.0f);

		// 죽음 몽타주 재생.
		const float PlayRate = 1.0f;
		AnimInstance->Montage_Play(MonsterDeadMontage, PlayRate);
	}
}
