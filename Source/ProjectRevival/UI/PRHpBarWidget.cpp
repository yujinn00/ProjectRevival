// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PRHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/PRCharacterWidgetInterface.h"

UPRHpBarWidget::UPRHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 일부러 유효하지 않은 값을 구분하기 위해 값 설정.
	MaxHp = -1.0f;
}

void UPRHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	// MaxHp 값이 제대로 설정됐는지 확인.
	ensure(MaxHp > 0.0f);

	// 프로그레스바 위젯 업데이트.
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}

void UPRHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이 함수가 호출되면, UI에 대한 대부분의 값이 초기화 되었다고 생각할 수 있음.

	// 위젯 참조 설정을 위해 이름으로 검색.
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	IPRCharacterWidgetInterface* CharacterWidget = Cast<IPRCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetUpCharacterWidget(this);
	}
}
