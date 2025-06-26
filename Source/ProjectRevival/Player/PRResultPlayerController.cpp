// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PRResultPlayerController.h"
#include "UI/PRUserWidget.h"

void APRResultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 결과 위젯이 지정되어 있다면 생성하고 화면에 추가.
	if (ResultWidgetClass)
	{
		UPRUserWidget* LobbyWidget = Cast<UPRUserWidget>(CreateWidget(this, ResultWidgetClass));
		LobbyWidget->AddToViewport();
	}

	// 입력 모드를 UI 전용으로 바꾸고, 마우스 커서를 표시.
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
