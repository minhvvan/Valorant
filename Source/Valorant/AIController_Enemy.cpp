// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AAIController_Enemy::AAIController_Enemy()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Enemy.BB_Enemy'"));
	if (BD.Succeeded())
	{
		BD_Enemy = BD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Enemy.BT_Enemy'"));
	if (BT.Succeeded())
	{
		BT_Enemy = BT.Object;
	}
}

void AAIController_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BD_Enemy, BlackboardComp))
	{
		if (RunBehaviorTree(BT_Enemy))
		{

		}
	}
	this->Blackboard = BlackboardComp;
}

void AAIController_Enemy::OnUnPossess()
{
}
