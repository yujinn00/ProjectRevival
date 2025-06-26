// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PRMonsterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPRMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};

// 공격 종료 델리게이트.
DECLARE_DELEGATE(FAIMonsterAttackFinished);

/**
 * 
 */
class PROJECTREVIVAL_API IPRMonsterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAIAttackRadius() = 0;
	virtual float GetAITurnSpeed() = 0;

	// 몬스터에서 델리게이트를 넘길 때 사용할 함수.
	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) = 0;

	// AI가 공격을 할 때 사용할 함수.
	virtual void AttackByAI() = 0;
};
