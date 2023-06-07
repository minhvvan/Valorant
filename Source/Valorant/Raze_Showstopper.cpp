// Fill out your copyright notice in the Description page of Project Settings.


#include "Raze_Showstopper.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ValorantCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Raze_Showstopper_Rocket.h"

ARaze_Showstopper::ARaze_Showstopper()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	InitialLifeSpan = 0;

	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);
}

void ARaze_Showstopper::Fire(FVector Direction)
{
	//fire rocket
	FRotator rotator;
	FVector spawnLocation = GetActorLocation();
	if (Character)
	{
		spawnLocation += Character->GetActorForwardVector() * 100;
	}

	FTransform SpawnTransform(rotator, spawnLocation);
	if (RocketClass)
	{
		auto Rocket = GetWorld()->SpawnActorDeferred<ARaze_Showstopper_Rocket>(RocketClass, SpawnTransform);
		Rocket->SetCharacter(Character);
		Rocket->FinishSpawning(SpawnTransform);

		Rocket->Fire(Direction);

		//add force character
		auto Dir = -Direction * RecoilStrength;
		auto Movement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
		Movement->AddImpulse(Dir, true);
	}

	Expire();
}

void ARaze_Showstopper::Expire()
{
	if (Character)
	{
		Character->ShowStopper = nullptr;
	}

	Destroy();

	//Dettach Timer UI

}

void ARaze_Showstopper::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &ARaze_Showstopper::Expire, PendingTime, false);
	//attach Timer UI
}
