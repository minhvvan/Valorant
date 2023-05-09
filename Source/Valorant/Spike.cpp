// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"
#include "TP_PickUpComponent.h"
#include "ValorantCharacter.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"
#include "TP_InteractionComponent.h"

// Sets default values
ASpike::ASpike()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUpComp = CreateDefaultSubobject<UTP_PickUpComponent>(TEXT("TP_PickUp"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(PickUpComp);
	Mesh->SetupAttachment(RootComponent);

	InteractComp = CreateDefaultSubobject<UTP_InteractionComponent>(TEXT("TP_Interact"));
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI_Interact"));

	InteractComp->SetupAttachment(RootComponent);
	InteractUI->SetupAttachment(InteractComp);

	bCanInteraction = false;
}

// Called when the game starts or when spawned
void ASpike::BeginPlay()
{
	Super::BeginPlay();
}

void ASpike::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PickUpComp->OnPickUp.AddUObject(this, &ASpike::PickUp);
	InteractComp->OnInteract.AddUObject(this, &ASpike::Interact);
	InteractComp->OnInteractEnd.AddUObject(this, &ASpike::EndInteract);
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
		SetCanInteraction(false);

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("Spike")));

		SetActorHiddenInGame(true);
	}
}

void ASpike::Interact(AValorantCharacter* Character)
{
	if (bCanInteraction)
	{
		UE_LOG(LogTemp, Warning, TEXT("INTERACT"));
		InteractUI->SetVisibility(true);

		if (Character)
		{
			OverlappedCharacter = Character;
			Character->bCanUnInstall = true;
		}
	}
}

void ASpike::EndInteract()
{
	if (OverlappedCharacter)
	{
		OverlappedCharacter->bCanUnInstall = false;
		OverlappedCharacter = nullptr;
	}

	if (bCanInteraction)
	{
		UE_LOG(LogTemp, Warning, TEXT("END INTERACT"));
		InteractUI->SetVisibility(false);
	}
}
