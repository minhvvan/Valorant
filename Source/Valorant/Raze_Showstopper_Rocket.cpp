// Fill out your copyright notice in the Description page of Project Settings.


#include "Raze_Showstopper_Rocket.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ValorantCharacter.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

ARaze_Showstopper_Rocket::ARaze_Showstopper_Rocket()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	InitialLifeSpan = 0;
}

void ARaze_Showstopper_Rocket::Fire(FVector Direction)
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	ProjectileMovement->SetActive(true);
}

void ARaze_Showstopper_Rocket::Explosion()
{
	CheckHit();
	PaintDecal();
	Destroy();
}

void ARaze_Showstopper_Rocket::PaintDecal()
{
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
		decal->SetDecalMaterial(Paint);
		decal->SetLifeSpan(4.0f);
		decal->GetDecal()->DecalSize = FVector(Range, Range, Range);
	}
}

void ARaze_Showstopper_Rocket::CheckHit()
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
			if (Victim == Character) continue;
			
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

void ARaze_Showstopper_Rocket::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSphereBeginOverlap"));
	Explosion();
}

void ARaze_Showstopper_Rocket::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComp->OnComponentHit.AddDynamic(this, &ARaze_Showstopper_Rocket::OnHit);
}

