// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanExplosion.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ValorantCharacter.h"
#include "AIController_Raze_BoomBot.h"
#include "DrawDebugHelpers.h"
#include "Raze_BoomBot.h"

UBTDecorator_CanExplosion::UBTDecorator_CanExplosion()
{
	NodeName = TEXT("CanExplosion");
}

bool UBTDecorator_CanExplosion::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto Target = Cast<AValorantCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
	{
		return false;
	}

	auto TargetLoc = Target->GetActorLocation();
	auto BoomBotLoc = CurrentPawn->GetActorLocation();

	TargetLoc.Z = 0;
	BoomBotLoc.Z = 0;

	auto Dist = FVector::Distance(TargetLoc, BoomBotLoc);
	
	return Result && Dist <= Range;
}
