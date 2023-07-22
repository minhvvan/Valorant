// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CanShoot.h"
#include "AIController_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ValorantCharacter.h"
#include "DrawDebugHelpers.h"
#include "Enemy.h"
#include "WeaponManager.h"


UBTService_CanShoot::UBTService_CanShoot()
{
	NodeName = TEXT("Check Can Shoot");
	Interval = .2f;
}

void UBTService_CanShoot::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DelataSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) return;

	auto World = GetWorld();
	auto Center = CurrentPawn->GetActorLocation();
	auto Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Enemy")));

	if (World == nullptr) return;
	if (Target == nullptr) return;

	auto EndLoc = Cast<AValorantCharacter>(Target)->GetActorLocation();

	FHitResult HitResult;
	bool Result = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Center,
		EndLoc,
		ECC_GameTraceChannel6
	);

	if (Result)
	{
		auto Victim = HitResult.GetActor();
		auto comp = HitResult.GetComponent();

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Victim->GetName()));

		if (AValorantCharacter* Player = Cast<AValorantCharacter>(Victim))
		{
			//DrawDebugLine(GetWorld(), Center, EndLoc, FColor::Red, true, -1, 0, .5f);

			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("CanShoot")), true);
			return;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("CanShoot")), false);
	return;
}
