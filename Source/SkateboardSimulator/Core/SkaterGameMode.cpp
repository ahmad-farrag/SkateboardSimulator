// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkaterGameMode.h"
#include "SkateboardSimulator/Characters/SkaterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASkaterGameMode::ASkaterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SkateboardSimulator/Characters/Blueprint/BP_SkaterCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
