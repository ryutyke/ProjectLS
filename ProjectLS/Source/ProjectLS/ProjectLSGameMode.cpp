// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectLSGameMode.h"
#include "ProjectLSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectLSGameMode::AProjectLSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
