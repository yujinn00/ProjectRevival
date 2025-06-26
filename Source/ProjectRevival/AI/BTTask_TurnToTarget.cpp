// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "PRAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/PRMonsterAIInterface.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	// 에디터에서 보이는 노드 이름 설정.
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 비헤이비어 트리를 소유하는 컨트롤러가 제어하는 폰 정보 가져오기.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 타겟 플레이어 정보 가져오기.
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 인터페이스 형 변환.
	IPRMonsterAIInterface* AIPawn = Cast<IPRMonsterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 회전할 속도 값.
	float TurnSpeed = AIPawn->GetAITurnSpeed();

	// 바라볼 방향 구하기.
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();

	// 높이 성분만 제거.
	LookVector.Z = 0.0f;

	// 바라볼 방향을 이용해 FRotator 구하기.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// 현재 프레임에 설정할 회전 값.
	FRotator CurrentRot = FMath::RInterpTo(
		ControllingPawn->GetActorRotation(),
		TargetRot,
		GetWorld()->DeltaTimeSeconds,
		TurnSpeed
	);

	// 회전 값 설정.
	ControllingPawn->SetActorRotation(CurrentRot);

	// 종료.
	return EBTNodeResult::Succeeded;
}
