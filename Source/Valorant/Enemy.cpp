// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "WeaponManager.h"
#include "StatComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Weapon.h"
#include "WeaponManager.h"
#include "Gun.h"
#include "TP_PickUpComponent.h"
#include "TP_InteractionComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->SetupAttachment(RootComponent);
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;
	Mesh1P->SetRelativeLocation(FVector(-40.f, 0.f, -90.f));

	HitboxHead->SetupAttachment(RootComponent);
	HitboxBody->SetupAttachment(RootComponent);
	HitboxRArm->SetupAttachment(RootComponent);
	HitboxLArm->SetupAttachment(RootComponent);
	HitboxRLeg->SetupAttachment(RootComponent);
	HitboxLLeg->SetupAttachment(RootComponent);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	if (HitboxHead)
	{
		HitboxHead->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("HeadHitbox")));
	}

	if (HitboxBody)
	{
		HitboxBody->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("BodyHitbox")));
	}

	if (HitboxRArm)
	{
		HitboxRArm->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("RightArmHitbox")));
	}

	if (HitboxLArm)
	{
		HitboxLArm->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("LeftArmHitbox")));
	}

	if (HitboxRLeg)
	{
		HitboxRLeg->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("RightLegHitbox")));
	}

	if (HitboxLLeg)
	{
		HitboxLLeg->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("LeftLegHitbox")));
	}

	WeaponClass = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/FirstPerson/Blueprints/BP_Rifle.BP_Rifle'")));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//auto HP = Stat->GetHp();

	//Vandal »ý¼º -> ºÎÂø
	FRotator rotator;
	FVector spawnLocation = GetActorLocation() + (10, 10, 10);
	FTransform SpawnTransform(rotator, spawnLocation);
	AWeapon* Vandal = GetWorld()->SpawnActorDeferred<AWeapon>(WeaponClass->GeneratedClass, SpawnTransform);
	if (Vandal)
	{
		Cast<AGun>(Vandal)->SetWeaponName(TEXT("Vandal"));
		Cast<AGun>(Vandal)->PickUpComp->SetGenerateOverlapEvents(false);
		Cast<AGun>(Vandal)->InteractComp->SetGenerateOverlapEvents(false);
		Vandal->WeaponTag = TEXT("Primary");
		Cast<AGun>(Vandal)->SetMesh(FString(TEXT("Vandal")));

		Vandal->FinishSpawning(SpawnTransform);
		WeaponManager->AddWeapon(Vandal);

		SetCurrentWeapon(Vandal);
		bHasRifle = true;
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
