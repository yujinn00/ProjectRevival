// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PRWidgetComponent.h"
#include "PRUserWidget.h"

void UPRWidgetComponent::InitWidget()
{
	// Super::InitWidget() 함수가 호출되면 내부에서 WidgetClass 정보를 바탕으로 CreateWidget() 함수를 호출하여 위젯 인스턴스를 생성함.
	Super::InitWidget();

	// Super::InitWidget() 함수 호출 이후에는 위젯 인스턴스가 있다고 확인할 수 있음.
	UPRUserWidget* PRUserWidget = Cast<UPRUserWidget>(GetWidget());
	if (PRUserWidget)
	{
		PRUserWidget->SetOwningActor(GetOwner());
	}
}
