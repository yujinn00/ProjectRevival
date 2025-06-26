// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PRAnimationSkillInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPRAnimationSkillInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREVIVAL_API IPRAnimationSkillInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// EventTag를 키로 사용하여 현재 무기에 맞는 스킬 데이터를 찾아 타격 판정을 처리하는 함수.
	virtual void SkillHitCheck(FName EventTag) = 0;
};
