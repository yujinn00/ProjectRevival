// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PRPlayerController.h"

void APRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
