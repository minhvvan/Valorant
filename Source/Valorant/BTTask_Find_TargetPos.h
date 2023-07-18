// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Find_TargetPos.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UBTTask_Find_TargetPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Find_TargetPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NedeMemory);
};
