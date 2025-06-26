// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PRUserWidget.h"

#include "Interface/PRCharacterWidgetInterface.h"

void UPRUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 예외 처리.
	if (!OwningActor)
	{
		OwningActor = GetOwningPlayerPawn();
	}
	
	IPRCharacterWidgetInterface* CharacterWidget = Cast<IPRCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetUpCharacterWidget(this);
	}
}
