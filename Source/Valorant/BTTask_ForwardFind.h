// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ForwardFind.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UBTTask_ForwardFind : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ForwardFind();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemoey) override;
};
