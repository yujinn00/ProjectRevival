// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PRGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 게임 클리어 여부를 저장할 변수.
	UPROPERTY(BlueprintReadWrite)
	uint8 bIsGameCleared : 1;

	// 최종 점수를 저장할 변수.
	UPROPERTY(BlueprintReadWrite)
	int32 FinalScore = 0;
};
