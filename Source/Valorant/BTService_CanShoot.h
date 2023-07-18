// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CanShoot.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UBTService_CanShoot : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CanShoot();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds) override;
};
