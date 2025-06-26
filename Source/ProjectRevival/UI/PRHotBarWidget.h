// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PRUserWidget.h"
#include "PRHotBarWidget.generated.h"

enum class EWeaponType : uint8;
/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UPRHotBarWidget : public UPRUserWidget
{
	GENERATED_BODY()

public:
	UPRHotBarWidget(const FObjectInitializer& ObjectInitializer);

	// 캐릭터의 무기 변경 신호를 받았을 때 외부에서 호출될 함수.
	UFUNCTION()
	void UpdateWeaponSlot(const class UPRWeaponData* CurrentWeaponData, EWeaponType CurrentWeaponType);

protected:
	// UMG가 초기화될 때 호출되는 함수.
	virtual void NativeConstruct() override;

protected:
	// 주먹 슬롯의 아이콘 이미지.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> FistImage;

	// 검 슬롯의 아이콘 이미지.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SwordImage;

	// 마법 슬롯의 아이콘 이미지.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MagicImage;

	// 스킬 슬롯의 아이콘 이미지.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage;

	// 구르기 슬롯의 아이콘 이미지.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> RollImage;

	// 에디터에서 할당할 주먹 슬롯에 표시될 아이콘.
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> FistIcon;

	// 에디터에서 할당할 검 슬롯에 표시될 아이콘.
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> SwordIcon;

	// 에디터에서 할당할 마법 슬롯에 표시될 아이콘.
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> MagicIcon;

	// 스킬 슬롯에 표시될 아이콘은 PRWeaponData에서 동적으로 할당.

	// 에디터에서 할당할 구르기 슬롯에 표시될 아이콘.
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> RollIcon;

private:
	// 활성화 상태의 색상을 저장할 변수.
	FLinearColor EnableColor;

	// 비활성화 상태의 색상을 저장할 변수.
	FLinearColor DisableColor;
};
