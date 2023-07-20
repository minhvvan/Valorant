// Copyright Epic Games, Inc. All Rights Reserved.

#include "ValorantGameMode.h"
#include "ValorantCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/TriggerVolume.h"
#include "BaseGameState.h"

AValorantGameMode::AValorantGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter'"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	GameStateClass = ABaseGameState::StaticClass();
}
