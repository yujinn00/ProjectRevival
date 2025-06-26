// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PRUserWidget.h"
#include "PRStatusBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRStatusBarWidget : public UPRUserWidget
{
	GENERATED_BODY()

public:
	// 캐릭터에서 호출할 체력바 업데이트 함수.
	UFUNCTION()
	void UpdateHpBar(float CurrentHp, float MaxHp);

protected:
	// UMG 에디터의 HP ProgressBar 위젯과 바인딩될 변수.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HP;

	// // UMG 에디터의 MP ProgressBar 위젯과 바인딩될 변수.
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<class UProgressBar> MP;
};
