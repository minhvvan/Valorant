// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "TP_PickUpComponent.h"
#include "TP_InteractionComponent.h"
#include "TP_WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComp = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("TP_Weapon"));
	SetRootComponent(WeaponComp);

	PickUpComp = CreateDefaultSubobject<UTP_PickUpComponent>(TEXT("TP_PickUp"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	InteractComp = CreateDefaultSubobject<UTP_InteractionComponent>(TEXT("TP_Interact"));
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI_Interact"));
	
	PickUpComp->SetupAttachment(RootComponent);
	Mesh->SetupAttachment(PickUpComp);

	InteractComp->SetupAttachment(RootComponent);
	InteractUI->SetupAttachment(InteractComp);

	bCanInteraction = false;
	WeaponTag = FName("Primary");
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PickUpComp->OnPickUp.AddUObject(this, &AWeapon::PickUp);
	InteractComp->OnInteract.AddUObject(this, &AWeapon::Interact);
	InteractComp->OnInteractEnd.AddUObject(this, &AWeapon::EndInteract);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::PickUp(AValorantCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("PickUp"));

	if (WeaponComp->ComponentHasTag(WeaponTag))
	{
		//!TODO: 1차적으로 Rifle로 --> Pistol도 가능하게 확장해야함
		if (Character->GetHasRifle())
		{
			UE_LOG(LogTemp, Warning, TEXT("Already Exist"));
		}
		else 
		{
			WeaponComp->AttachWeapon(Character);
			InteractComp->SetGenerateOverlapEvents(false);
		}
	}
}

void AWeapon::Interact(AValorantCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact"));

	if (Character->GetHasRifle())
	{
		bCanInteraction = true;
		InteractUI->SetVisibility(true);
	}
}

void AWeapon::EndInteract()
{
	bCanInteraction = false;
	InteractUI->SetVisibility(false);
}

