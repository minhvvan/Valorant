// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "TP_PickUpComponent.h"
#include "TP_InteractionComponent.h"
#include "TP_WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

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

	if (bCanInteraction)
	{
		auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (Controller->WasInputKeyJustPressed(EKeys::E))
		{
			//detach weapon
			if (OverlappedCharacter)
			{
				OverlappedCharacter->RemoveFromWeapon(WeaponTag.ToString());
				OverlappedCharacter->DetachWeapon(WeaponTag.ToString());
				OverlappedCharacter->AddToWeapon(WeaponTag.ToString(), this);
				WeaponComp->AttachWeapon(OverlappedCharacter, type);
				UE_LOG(LogTemp, Warning, TEXT("Swap"));
			}
		}
	}
}

void AWeapon::PickUp(AValorantCharacter* Character)
{
	if (ActorHasTag(WeaponTag))
	{
		if (WeaponTag == "Primary")
		{
			if (Character->GetHasRifle())
			{
				UE_LOG(LogTemp, Warning, TEXT("Already Exist: Primary"));
			}
			else
			{
				Character->AddToWeapon(WeaponTag.ToString(), this);
				WeaponComp->AttachWeapon(Character, PRIMARY);
				InteractComp->SetGenerateOverlapEvents(false);
			}
		}
		else if (WeaponTag == "Secondary")
		{
			if (Character->GetHasPistol())
			{
				UE_LOG(LogTemp, Warning, TEXT("Already Exist: Secondary"));
			}
			else
			{
				Character->AddToWeapon(WeaponTag.ToString(), this);
				WeaponComp->AttachWeapon(Character, SECONDARY);
				InteractComp->SetGenerateOverlapEvents(false);
			}
		}
	}
}

void AWeapon::Interact(AValorantCharacter* Character)
{
	if (Character->GetHasRifle())
	{
		OverlappedCharacter = Character;
		bCanInteraction = true;
		InteractUI->SetVisibility(true);
	}
}

void AWeapon::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("End Overlap"));

	OverlappedCharacter = nullptr;
	bCanInteraction = false;
	InteractUI->SetVisibility(false);
}

void AWeapon::DetachWeapon()
{
	if (WeaponComp)
	{
		WeaponComp->DetachWeapon(type);
	}
}

void AWeapon::EnableInteraction()
{
	InteractComp->SetGenerateOverlapEvents(true);
}

