// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PRAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API APRAIController : public AAIController
{
	GENERATED_BODY()

public:
	APRAIController();

	// AI 실행하는 함수.
	void RunAI();

	// AI 중지하는 함수.
	void StopAI();

protected:
	// 컨트롤러가 폰에 빙의했을 때 실행되는 이벤트 함수. 
	virtual void OnPossess(APawn* InPawn) override;

// Data Section.
private:
	// Blackboard 에셋.
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	// Behavior Tree 에셋.
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
