// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Aiming.h"
#include "AIController_Enemy.h"
#include "Enemy.h"
#include "TP_WeaponComponent.h"
#include "Gun.h"
#include "Weapon.h"
#include "Components/Viewport.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Aiming::UBTService_Aiming()
{
	NodeName = TEXT("Aiming");
}

void UBTService_Aiming::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentPawn == nullptr) return;

	auto Target = Cast<AValorantCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Enemy"))));
	if (Target == nullptr) return;

	auto Dir = Target->GetActorLocation() - CurrentPawn->GetActorLocation();

	auto offset = FMath::Rand() % 50;
	offset += 50;
	Dir.Z -= offset;

	auto Aim = CurrentPawn->GetActorLocation() + Dir;

	//UE_LOG(LogTemp, Warning, TEXT("Target: %s"), *FString(Target->GetActorLocation().ToString()));
	//UE_LOG(LogTemp, Warning, TEXT("Aim: %s"), *FString(Aim.ToString()));

	FRotator Rot = FRotationMatrix::MakeFromX(Dir).Rotator();

	FRotator SmoothedRotation = FMath::RInterpTo(CurrentPawn->GetViewRotation(), Rot, GetWorld()->GetDeltaSeconds(), AimSpeed);
	CurrentPawn->SetActorRotation(SmoothedRotation);

	return;
}
