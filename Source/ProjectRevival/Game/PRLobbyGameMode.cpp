// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PRLobbyGameMode.h"

APRLobbyGameMode::APRLobbyGameMode()
{
	// 로비에서 사용할 플레이어 컨트롤러를 지정.
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/ProjectRevival/PlayerController/BP_PRLobbyPlayerController.BP_PRLobbyPlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	// 로비에서는 플레이어 캐릭터가 스폰되지 않도록 DefaultPawnClass를 null로 설정.
	DefaultPawnClass = nullptr;
}
