// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PRUserWidget.h"
#include "PRHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRHpBarWidget : public UPRUserWidget
{
	GENERATED_BODY()

public:
	UPRHpBarWidget(const FObjectInitializer& ObjectInitializer);

	// 최대 체력 설정 함수.
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

	// HpBar에 퍼센트를 조절할 때 호출되는 함수.
	void UpdateHpBar(float NewCurrentHp);

protected:
	// UMG가 초기화될 때 호출되는 함수.
	virtual void NativeConstruct() override;

protected:
	// HpBar를 보여주기 위해 사용할 프로그레스바 참조 변수.
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	// 최대 체력 값.
	UPROPERTY()
	float MaxHp;
};
