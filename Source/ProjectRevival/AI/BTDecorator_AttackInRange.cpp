// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "Interface/PRMonsterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PRAI.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	// 에디터에서 보이는 노드 이름 설정.
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 비헤이비어 트리를 소유하는 컨트롤러가 제어하는 폰 정보 가져오기.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return false;
	}

	// 인터페이스로 형 변환.
	IPRMonsterAIInterface* AIPawn = Cast<IPRMonsterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return false;
	}

	// 블랙보드에 저장된 캐릭터 값.
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!Target)
	{
		return false;
	}

	// 캐릭터와의 거리 계산.
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

	// 공격 범위 값.
	float AttackRangeWidthRadius = AIPawn->GetAIAttackRange();

	// 캐릭터와의 거리가 공격 가능 거리보다 가까운지 확인 후 결과 반환.
	bResult = (DistanceToTarget <= AttackRangeWidthRadius);

	return bResult;
}
