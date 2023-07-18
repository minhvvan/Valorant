// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindEnemy.h"
#include "AIController_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ValorantCharacter.h"
#include "DrawDebugHelpers.h"
#include "Enemy.h"


UBTService_FindEnemy::UBTService_FindEnemy()
{
	NodeName = TEXT("FindEnemy");
	Interval = 1.f;
}

void UBTService_FindEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DelataSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) return;

	auto World = GetWorld();
	auto Center = CurrentPawn->GetActorLocation();
	auto SerchRadius = Cast<AEnemy>(CurrentPawn)->GetSearchRadius();
	auto FowardVector = CurrentPawn->GetActorForwardVector();
	auto FOV = Cast<AEnemy>(CurrentPawn)->GetFOV();

	if (World == nullptr) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	bool Result = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel5,
		FCollisionShape::MakeSphere(SerchRadius),
		QueryParams
	);

	if (Result)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			AValorantCharacter* Player = Cast<AValorantCharacter>(OverlapResult.GetActor());
			if (Player && Player->GetController()->IsPlayerController())
			{
				//시야각 판별
				auto ToEnemy = (Player->GetActorLocation() - Center);
				ToEnemy.Normalize();

				auto dotEnemy = FowardVector.Dot(ToEnemy);
				float theta = FMath::RadiansToDegrees(FMath::Acos(dotEnemy));

				if (theta <= FOV / 2.f) 
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Enemy")), Player);
					
					return;
				}
				//UE_LOG(LogTemp, Warning, TEXT("%f"), theta);
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Enemy")), nullptr);
	//DrawDebugSphere(World, Center, SerchRadius, 16, FColor::Red, false, 0.2f);
	return;
}
