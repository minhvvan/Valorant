// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ValorantCharacter.h"
#include "AIController_Raze_BoomBot.h"
#include "DrawDebugHelpers.h"
#include "Raze_BoomBot.h"


UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget"); 
	Interval = 1.f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = Cast<ARaze_BoomBot>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentPawn == nullptr)
	{
		return;
	}

	auto World = CurrentPawn->GetWorld();
	auto Center = CurrentPawn->GetActorLocation();
	float SearchRadius = Range;

	if (World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	auto bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams
	);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			auto Character = Cast<AValorantCharacter>(OverlapResult.GetActor());
			//Owner Á¦¿ÜÇØ¾ßµÊ
			if (Character && CurrentPawn->GetCharacter() != Character)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Character);
				
				//DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
				return;
			}
		}

		//DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
		//	DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
}
