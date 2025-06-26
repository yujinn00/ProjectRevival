// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PRHotBarWidget.h"
#include "Components/Image.h"
#include "Weapon/PRWeaponData.h"
#include "Character/PRCharacterBase.h"

UPRHotBarWidget::UPRHotBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 활성화 시 사용할 색상을 흰색으로 정의.
	EnableColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 비활성화 시 사용할 색상을 회색으로 정의.
	DisableColor = FLinearColor(0.1f, 0.1f, 0.1f, 0.6f);
}

void UPRHotBarWidget::UpdateWeaponSlot(const UPRWeaponData* CurrentWeaponData, EWeaponType CurrentWeaponType)
{
	// 스킬 아이콘 업데이트.
	if (SkillImage)
	{
		// 현재 무기 데이터에 스킬 아이콘이 지정되어 있는지 확인.
		if (CurrentWeaponData && CurrentWeaponData->SkillIcon)
		{
			// 지정된 아이콘으로 이미지를 교체하고, 보이도록 설정.
			SkillImage->SetBrushFromTexture(CurrentWeaponData->SkillIcon);
			SkillImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			// 스킬 아이콘이 없다면 이미지를 숨김.
			SkillImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// 무기 슬롯 테두리 활성화 및 비활성화 처리.
	if (FistImage)
	{
		FistImage->SetColorAndOpacity(CurrentWeaponType == EWeaponType::Fist ? EnableColor : DisableColor);
	}
	if (SwordImage)
	{
		SwordImage->SetColorAndOpacity(CurrentWeaponType == EWeaponType::Sword ? EnableColor : DisableColor);
	}
	if (MagicImage)
	{
		MagicImage->SetColorAndOpacity(CurrentWeaponType == EWeaponType::Magic ? EnableColor : DisableColor);
	}
}

void UPRHotBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯이 처음 생성될 때, 각 슬롯에 기본 아이콘 설정.
	if (FistImage && FistIcon)
	{
		FistImage->SetBrushFromTexture(FistIcon);
	}
	if (SwordImage && SwordIcon)
	{
		SwordImage->SetBrushFromTexture(SwordIcon);
	}
	if (MagicImage && MagicIcon)
	{
		MagicImage->SetBrushFromTexture(MagicIcon);
	}
	if (RollImage && RollIcon)
	{
		RollImage->SetBrushFromTexture(RollIcon);
	}
}
