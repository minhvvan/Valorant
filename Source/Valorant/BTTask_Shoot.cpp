// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController_Enemy.h"
#include "Enemy.h"
#include "TP_WeaponComponent.h"
#include "Gun.h"
#include "Weapon.h"
#include "Components/Viewport.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"


UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CurrentPawn = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentPawn == nullptr) return EBTNodeResult::Failed;

	auto Weapon = Cast<AGun>(CurrentPawn->GetCurrentWeapon());
	if (Weapon == nullptr) return EBTNodeResult::Failed;
	if(Weapon->WeaponComp == nullptr) return EBTNodeResult::Failed;

	//auto Rand = FMath::Rand() % 5;
	//for (int i = 0; i < Rand; i++)
	Weapon->WeaponComp->Fire();

	return EBTNodeResult::Succeeded;
}
