// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LSGameMode.h"

ALSGameMode::ALSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ProjectLS/Blueprint/BP_LSCharacterPlayer.BP_LSCharacterPlayer_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
