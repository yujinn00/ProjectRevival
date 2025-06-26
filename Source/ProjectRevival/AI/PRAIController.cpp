// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PRAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PRAI.h"

APRAIController::APRAIController()
{
	// BlackBoard 에셋 로드.
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/ProjectRevival/AI/BB_PRMonster.BB_PRMonster"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	// Behavior Tree 에셋 로드.
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/ProjectRevival/AI/BT_PRMonster.BT_PRMonster"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void APRAIController::RunAI()
{
	// 블랙보드 컴포넌트 포인터 가져오기.
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	// 사용할 블랙보드 지정.
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 시작할 때 NPC의 위치를 블랙보드에 HomePos에 저장.
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// 행동 트리 실행.
		bool RunResult = RunBehaviorTree(BTAsset);

		// 실행에 실패하면 확인.
		ensure(RunResult);
	}
}

void APRAIController::StopAI()
{
	// 중지를 위해 실행 중인 BehaviorTreeComponent 가져오기.
	// BrainComponent는 AI의 의사 결정 로직을 담당하는 컴포넌트로, BehaviorTree를 비롯한 여러 종류의 브레인 시스템을 포함함.
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);

	// 행동 트리 중지.
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void APRAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 컨트롤러가 폰에 빙의하면 AI를 실행.
	RunAI();
}
