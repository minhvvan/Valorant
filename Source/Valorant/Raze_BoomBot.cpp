// Fill out your copyright notice in the Description page of Project Settings.


#include "Raze_BoomBot.h"
#include "AIController_Raze_BoomBot.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ValorantCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/PawnMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ARaze_BoomBot::ARaze_BoomBot()
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

	AIControllerClass = AAIController_Raze_BoomBot::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARaze_BoomBot::Fire(FVector Direction)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire"));
	//AI Possess
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->Possess(this);
	}

	//timer 설정
}

void ARaze_BoomBot::Explosion()
{
	UE_LOG(LogTemp, Warning, TEXT("EXPLOSION"));
	GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
	//Boom
	CheckHit();
	Destroy();
}

void ARaze_BoomBot::CheckHit()
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

void ARaze_BoomBot::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//방향 변경
	auto hitNormal = Hit.ImpactNormal;
	auto Dir = GetActorForwardVector();

	auto Result = Dir + 2 * hitNormal * FVector::DotProduct(-Dir, hitNormal);
	SetActorRotation(Result.Rotation());
}

void ARaze_BoomBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComp->OnComponentHit.AddDynamic(this, &ARaze_BoomBot::OnHit);
}

void ARaze_BoomBot::Tick(float DeltaTime)
{
	auto Dir = GetActorForwardVector();
	auto Location = GetActorLocation();
	auto Target = Location + Dir * 50;

	auto ct = Cast<AAIController_Raze_BoomBot>(GetController());
	if (ct)
	{
		ct->GetBlackboardComponent()->SetValueAsVector(FName(TEXT("Forward")), Target);
	}
}
