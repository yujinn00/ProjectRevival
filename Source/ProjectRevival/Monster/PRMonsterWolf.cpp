// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/PRMonsterWolf.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/PRCollision.h"
#include "AI/PRAIController.h"

APRMonsterWolf::APRMonsterWolf()
{
	// 캡슐 컴포넌트 설정.
	// 지울 수 없는 컴포넌트.
	GetCapsuleComponent()->SetCapsuleHalfHeight(44.0f);

	// 박스 컴포넌트 설정.
	// 실질적인 콜리전 컴포넌트.
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(70.0f, 30.0f, 44.0f));
	CollisionBox->SetCollisionProfileName(CPROFILE_PRMONSTERCOLLISION);

	// Mesh.
	GetMesh()->SetRelativeLocationAndRotation(FVector(-15.0f, 0.0f, -44.0f), FRotator(0.0f, -90.0f, 0.0f));

	// 스켈레탈 메시 설정.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/AnimalVarietyPack/Wolf/Meshes/SK_Wolf.SK_Wolf"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// 애님 인스턴스 설정.
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ProjectRevival/Animation/Monster/Wolf/ABP_PRWolf.ABP_PRWolf_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}

float APRMonsterWolf::GetAIAttackRange()
{
	return Super::GetAIAttackRange();
}

void APRMonsterWolf::SetDead()
{
	Super::SetDead();

	// AI 명령 전달 중지.
	APRAIController* ABAIController = Cast<APRAIController>(GetController());
	if (ABAIController)
	{
		ABAIController->StopAI();
	}

	// 타이머를 사용해 액터 제거.
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DeadTimerHandle,
		FTimerDelegate::CreateLambda([&]()
			{
				// 액터 제거.
				Destroy();
			}
		),
		DeadEventDelayTime, // 타이머 설정 시간.
		false // 반복 여부 설정 (반복 안함).
	);
}
