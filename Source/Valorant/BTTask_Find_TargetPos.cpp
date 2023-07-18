// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Find_TargetPos.h"
#include "AIController_Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Enemy.h"

UBTTask_Find_TargetPos::UBTTask_Find_TargetPos()
{
	NodeName = TEXT("FindPos");
}

EBTNodeResult::Type UBTTask_Find_TargetPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NedeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NedeMemory);


	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	auto Enemy = Cast<AEnemy>(CurrentPawn);

	FNavLocation RandPos;
	if (NavSystem->GetRandomPointInNavigableRadius(CurrentPawn->GetActorLocation(), Enemy->GetSearchRadius(), RandPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("TargetPos")), RandPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
