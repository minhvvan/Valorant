// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Raze_BoomBot.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


AAIController_Raze_BoomBot::AAIController_Raze_BoomBot()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BDT(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Raze_BoomBot.BB_Raze_BoomBot'"));

	if (BDT.Succeeded())
	{
		BlackboardData = BDT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Raze_BoomBot.BT_Raze_BoomBot'"));
	{
		BehaviorTree = BT.Object;
	}
}

void AAIController_Raze_BoomBot::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BlackboardData, BlackboardComp))
	{
		if (RunBehaviorTree(BehaviorTree))
		{
			//TODO
		}
	}
}

void AAIController_Raze_BoomBot::OnUnPossess()
{
	Super::OnUnPossess();

}
