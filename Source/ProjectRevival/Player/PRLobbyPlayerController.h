// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PRLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API APRLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	// 에디터에서 지정할 로비 UI 위젯 블루프린트.
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UPRUserWidget> LobbyWidgetClass;
};
