// Fill out your copyright notice in the Description page of Project Settings.


#include "Raze_Grenade_Sub.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARaze_Grenade_Sub::ARaze_Grenade_Sub()
{
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp->InitSphereRadius(10.0f);
	//CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	//RootComponent = CollisionComp;

	//ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	//Mesh->SetupAttachment(RootComponent);

	InitialLifeSpan = 0;
}

void ARaze_Grenade_Sub::Explosion()
{
	GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
	//Boom
	CheckHit();
}
