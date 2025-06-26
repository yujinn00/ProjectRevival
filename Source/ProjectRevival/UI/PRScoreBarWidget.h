// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PRUserWidget.h"
#include "PRScoreBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRScoreBarWidget : public UPRUserWidget
{
	GENERATED_BODY()

public:
	// 외부에서 킬 스코어를 업데이트하기 위해 호출할 함수.
	UFUNCTION()
	void UpdateKillScore(int32 NewKillScore);

protected:
	// UMG의 숫자 텍스트 위젯과 바인딩될 변수.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Score;
};
