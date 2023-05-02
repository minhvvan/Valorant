// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "TP_PickUpComponent.h"
#include "TP_InteractionComponent.h"
#include "TP_WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AGun::AGun()
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

void AGun::BeginPlay()
{
	Super::BeginPlay();

}

void AGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PickUpComp->OnPickUp.AddUObject(this, &AGun::PickUp);
	InteractComp->OnInteract.AddUObject(this, &AGun::Interact);
	InteractComp->OnInteractEnd.AddUObject(this, &AGun::EndInteract);
}

// Called every frame
void AGun::Tick(float DeltaTime)
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
				WeaponComp->AttachWeapon(OverlappedCharacter, WeaponTag.ToString());
				UE_LOG(LogTemp, Warning, TEXT("Swap"));
			}
		}
	}
}

void AGun::PickUp(AValorantCharacter* Character)
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
				WeaponComp->AttachWeapon(Character, WeaponTag.ToString());
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
				WeaponComp->AttachWeapon(Character, WeaponTag.ToString());
				InteractComp->SetGenerateOverlapEvents(false);
			}
		}
	}
}

void AGun::Interact(AValorantCharacter* Character)
{
	if (Character->GetHasRifle())
	{
		OverlappedCharacter = Character;
		bCanInteraction = true;
		InteractUI->SetVisibility(true);
	}
}

void AGun::EndInteract()
{
	OverlappedCharacter = nullptr;
	bCanInteraction = false;
	InteractUI->SetVisibility(false);
}

void AGun::DetachWeapon()
{
	if (WeaponComp)
	{
		WeaponComp->DetachWeapon();
	}
}

void AGun::EnableInteraction()
{
	InteractComp->SetGenerateOverlapEvents(true);
}

void AGun::SetCanFire(bool Flag)
{
	if (WeaponComp)
	{
		WeaponComp->SetCanFire(Flag);
	}
}

void AGun::Drop()
{
	//detach weapon
	if (OverlappedCharacter)
	{
		OverlappedCharacter->RemoveFromWeapon(WeaponTag.ToString());
		OverlappedCharacter->DetachWeapon(WeaponTag.ToString());
	}
}

