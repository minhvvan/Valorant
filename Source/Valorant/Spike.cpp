// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"
#include "TP_PickUpComponent.h"
#include "ValorantCharacter.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"

// Sets default values
ASpike::ASpike()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUpComp = CreateDefaultSubobject<UTP_PickUpComponent>(TEXT("TP_PickUp"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(PickUpComp);
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpike::BeginPlay()
{
	Super::BeginPlay();

	PickUpComp->OnPickUp.AddUObject(this, &ASpike::PickUp);
}

// Called every frame
void ASpike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpike::PickUp(AValorantCharacter* Character)
{
	if (CanPickUp)
	{
		Character->SetSpike(this);

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("Spike")));

		SetActorHiddenInGame(true);
	}
}
