// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanExplosion.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UBTDecorator_CanExplosion : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CanExplosion();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;
	
	UPROPERTY(EditAnywhere)
	float Range = 50.f;
};
