// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API AAIController_Enemy : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_Enemy();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	class UBehaviorTree* BT_Enemy;

	UPROPERTY()
	class UBlackboardData* BD_Enemy;
};
