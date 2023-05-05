// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "Engine/EngineTypes.h"

void ABaseGameState::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseGameState::Expire, 30.0f, false);
}

void ABaseGameState::Expire()
{
	UE_LOG(LogTemp, Warning, TEXT("BOOOOOOOOOOOOOMMMM"));
	RedScore++;
}

void ABaseGameState::Clear()
{
	UE_LOG(LogTemp, Warning, TEXT("Clear Spike"));
	BlueScore++;
}

void ABaseGameState::CheckScore()
{
	if (RedScore == 10)
	{
		//Red ½Â
	}
	else if (BlueScore == 10)
	{
		//blue ½Â
	}
}

