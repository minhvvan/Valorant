// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Explosion.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ValorantCharacter.h"
#include "AIController_Raze_BoomBot.h"
#include "DrawDebugHelpers.h"
#include "Raze_BoomBot.h"

UBTTask_Explosion::UBTTask_Explosion()
{
	NodeName = TEXT("Explosion");
}

EBTNodeResult::Type UBTTask_Explosion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BoomBot = Cast<ARaze_BoomBot>(OwnerComp.GetAIOwner()->GetPawn());
	if (BoomBot)
	{
		BoomBot->Explosion();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}