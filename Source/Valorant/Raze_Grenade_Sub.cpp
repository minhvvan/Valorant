// Fill out your copyright notice in the Description page of Project Settings.


#include "Raze_Grenade_Sub.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

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

	{
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Resources/Raze/Grenade/Sub_Boom_Mat.Sub_Boom_Mat'"));
		if (Material.Object != NULL)
		{
			Paints.Add((UMaterial*)Material.Object);
		}
	}

	{
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Resources/Raze/Grenade/Sub_Boom_Mat2.Sub_Boom_Mat2'"));
		if (Material.Object != NULL)
		{
			Paints.Add((UMaterial*)Material.Object);
		}
	}

	{
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Resources/Raze/Grenade/Sub_Boom_Mat3.Sub_Boom_Mat3'"));
		if (Material.Object != NULL)
		{
			Paints.Add((UMaterial*)Material.Object);
		}
	}

	{
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Resources/Raze/Grenade/Sub_Boom_Mat4.Sub_Boom_Mat4'"));
		if (Material.Object != NULL)
		{
			Paints.Add((UMaterial*)Material.Object);
		}
	}

	{
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Resources/Raze/Grenade/Sub_Boom_Mat5.Sub_Boom_Mat5'"));
		if (Material.Object != NULL)
		{
			Paints.Add((UMaterial*)Material.Object);
		}
	}
}

void ARaze_Grenade_Sub::Explosion()
{
	GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
	//Boom
	CheckHit();
	
	//paint decal
	FVector ImpactPoint = GetActorLocation();
	FVector impactNormal = { 0,0,1 };

	FVector basis = FVector(0, 0, 1);
	if (fabsf(impactNormal.Y) > 0.8) {
		basis = FVector(1, 0, 1);
	}
	FVector right = FVector::CrossProduct(impactNormal, basis).GetUnsafeNormal();
	FVector forward = FVector::CrossProduct(right, impactNormal);
	FBasisVectorMatrix bvm(forward, right, impactNormal, FVector(0, 0, 0));
	FRotator theRotation = bvm.Rotator();

	//Decal
	ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(ImpactPoint, theRotation);
	if (decal)
	{
		auto idx = FMath::RandRange(0, 4);
		auto paint = Paints[idx];
		decal->SetDecalMaterial(Paints[idx]);
		decal->SetLifeSpan(2.0f);
		decal->GetDecal()->DecalSize = FVector(Range, Range, Range);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
	}


	Destroy();
}
