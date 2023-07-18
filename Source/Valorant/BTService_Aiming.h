// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Aiming.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UBTService_Aiming : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Aiming();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float AimSpeed = 500.f;
};
