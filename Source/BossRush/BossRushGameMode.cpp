// Copyright Epic Games, Inc. All Rights Reserved.

#include "BossRushGameMode.h"
#include "BossRushCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABossRushGameMode::ABossRushGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Fighter/BP_Fighter.BP_Fighter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
