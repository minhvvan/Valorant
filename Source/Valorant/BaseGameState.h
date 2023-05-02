// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/EngineTypes.h"
#include "BaseGameState.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	FTimerHandle TimerHandle;

	void StartTimer();
	void Expire();
	void Clear();
};
