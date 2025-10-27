// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameCombatEngineerGameMode.h"
#include "GameCombatEngineerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameCombatEngineerGameMode::AGameCombatEngineerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
