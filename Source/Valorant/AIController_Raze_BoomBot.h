// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Raze_BoomBot.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API AAIController_Raze_BoomBot : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_Raze_BoomBot();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UBlackboardData* BlackboardData;


};
