// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PRCharacterBase.h"
#include "PRCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API APRCharacterNonPlayer : public APRCharacterBase
{
	GENERATED_BODY()

public:
	APRCharacterNonPlayer();

// Dead Section.
protected:
	// 죽음 상태 설정 함수.
	void SetDead() override;
};
