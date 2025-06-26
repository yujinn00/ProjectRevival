// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PRResultGameMode.h"

APRResultGameMode::APRResultGameMode()
{
	// 결과에서 사용할 플레이어 컨트롤러를 지정.
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/ProjectRevival/PlayerController/BP_PRResultPlayerController.BP_PRResultPlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	// 결과에서는 플레이어 캐릭터가 스폰되지 않도록 DefaultPawnClass를 null로 설정.
	DefaultPawnClass = nullptr;
}
