// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ForwardFind.h"
#include "AIController_Raze_BoomBot.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_ForwardFind::UBTTask_ForwardFind()
{
	NodeName = TEXT("ForwardFind");
}

EBTNodeResult::Type UBTTask_ForwardFind::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemoey)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemoey);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("no pawn"));
		return EBTNodeResult::Failed;
	}

	//!ÇöÀç ¶Ò¶Ò ²÷±â¸é¼­ ÀÌµ¿ 
	return EBTNodeResult::Succeeded;
}
