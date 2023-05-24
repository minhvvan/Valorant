// Fill out your copyright notice in the Description page of Project Settings.


#include "Raze_Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "ValorantCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Raze_Grenade_Sub.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

ARaze_Grenade::ARaze_Grenade()
{
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	InitialLifeSpan = 0;

	dy = { 1, -1, -1, 1 };
	dx = { 1, 1, -1, -1 };
}


void ARaze_Grenade::Fire(FVector Direction)
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	ProjectileMovement->SetActive(true);

	//timer 설정
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &ARaze_Grenade::Explosion, PendingTime, false);
}

void ARaze_Grenade::Explosion()
{
	GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
	//Boom
	CheckHit();

	//Spawn 4 Small Grenade
	FRotator rotator;
	FVector spawnLocation = GetActorLocation();
	rotator.Pitch += 100;
	rotator.Yaw += 45;

	for (int i = 0; i < 4; i++)
	{
		auto newRot = rotator;
		auto newLoc = spawnLocation;
		newLoc.X += 10 * dx[i];
		newLoc.Y += 10 * dy[i];
		newRot.Yaw += 90 * i;

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(newRot.ToString()));

		FTransform SpawnTransform(newRot, newLoc);
		ARaze_Grenade_Sub* SubGrenade = GetWorld()->SpawnActorDeferred<ARaze_Grenade_Sub>(SubGrenadeClass, SpawnTransform);
		if (SubGrenade)
		{
			SubGrenade->SetCharacter(Character);
			SubGrenade->FinishSpawning(SpawnTransform);

			FVector LaunchDirection = newRot.Vector();
			SubGrenade->Fire(LaunchDirection);
		}
	}

	//paint decal
	FVector ImpactPoint = spawnLocation;
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
		decal->SetDecalMaterial(Paint);
		decal->SetLifeSpan(4.0f);
		decal->GetDecal()->DecalSize = FVector(Range, Range, Range);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
	}

 	Destroy();
}

void ARaze_Grenade::CheckHit()
{
	UWorld* World = GetWorld();
	FVector Center = GetActorLocation();

	if (World == nullptr) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, this);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(Range),
		QueryParams);


	//~같은 Actor가 다중 Hit됨 -> 변경 필요
	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			AValorantCharacter* Victim = Cast<AValorantCharacter>(OverlapResult.GetActor());
			if (Victim && !VictimSet.Contains(Victim->GetUniqueID()))
			{
				VictimSet.Add(Victim->GetUniqueID());
				DrawDebugSphere(World, Center, Range, 16, FColor::Blue, false, 1.2f);
				if (Character)
				{
					UGameplayStatics::ApplyDamage(Victim, Damage, Character->GetController(), Character, NULL);
				}
			}
		}
	}
}
	