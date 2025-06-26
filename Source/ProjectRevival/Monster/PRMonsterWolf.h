// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/PRMonsterBase.h"
#include "PRMonsterWolf.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API APRMonsterWolf : public APRMonsterBase
{
	GENERATED_BODY()

public:
	APRMonsterWolf();

// AI Section.
protected:
	virtual float GetAIAttackRange() override;

protected:
	// 죽음 상태 설정 함수.
	void SetDead() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<class UBoxComponent> CollisionBox;
};
