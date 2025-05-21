// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRevivalGameMode.h"
#include "ProjectRevivalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectRevivalGameMode::AProjectRevivalGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
