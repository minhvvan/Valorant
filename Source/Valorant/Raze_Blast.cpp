// Fill out your copyright notice in the Description page of Project Settings.


#include "Raze_Blast.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ValorantCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StatusEffect.h"

ARaze_Blast::ARaze_Blast()
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

	auto Flash = new SEFlash(2.f, 500.f, this);
	if (Flash)
	{
		SEList.Add(Flash);
	}
}

void ARaze_Blast::Fire(FVector Direction)
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	ProjectileMovement->SetActive(true);

	//timer 설정
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &ARaze_Blast::Explosion, PendingTime, false);
}

void ARaze_Blast::Explosion()
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
		decal->SetDecalMaterial(Paint);
		decal->SetLifeSpan(4.0f);
		decal->GetDecal()->DecalSize = FVector(Range, Range, Range);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
	}

	if (Character)
	{
		Character->Blast = nullptr;
	}

	for (auto SE : SEList)
	{
		SE->Fire();
		UE_LOG(LogTemp, Warning, TEXT("Fire"));
	}

	Destroy();
}

void ARaze_Blast::CheckHit()
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

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			AValorantCharacter* Victim = Cast<AValorantCharacter>(OverlapResult.GetActor());

			if (Victim && !VictimSet.Contains(Victim->GetUniqueID()))
			{
				//Add Force
				auto ForceDir = (Victim->GetActorLocation() - GetActorLocation()) * Power;
				auto Movement = Cast<UCharacterMovementComponent>(Victim->GetMovementComponent());
				Movement->AddImpulse(ForceDir, true);

				VictimSet.Add(Victim->GetUniqueID());
				DrawDebugSphere(World, Center, Range, 16, FColor::Blue, false, 1.2f);
				if (Character)
				{
					if (Victim != Character)
					{
						UGameplayStatics::ApplyDamage(Victim, Damage, Character->GetController(), Character, NULL);
					}
				}
			}
		}
	}
}

void ARaze_Blast::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//!error  ->  벽 계산이 잘 안됨
	FVector ImpactPoint = Hit.Location;
	FVector impactNormal = Hit.Normal;

	FVector basis = FVector(0, 0, 1);
	if (fabsf(impactNormal.Y) > 0.8) {
		basis = FVector(1, 0, 1);
	}
	FVector right = FVector::CrossProduct(impactNormal, basis).GetUnsafeNormal();
	FVector forward = FVector::CrossProduct(right, impactNormal);
	FBasisVectorMatrix bvm(forward, right, impactNormal, FVector(0, 0, 0));
	FRotator theRotation = bvm.Rotator();

	SetActorRotation(theRotation);
}

void ARaze_Blast::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComp->OnComponentHit.AddDynamic(this, &ARaze_Blast::OnHit);
}
