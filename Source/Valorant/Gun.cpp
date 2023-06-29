// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "TP_PickUpComponent.h"
#include "TP_InteractionComponent.h"
#include "TP_WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BulletComponent.h"
#include "WeaponManager.h"

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

	BulletComp = CreateDefaultSubobject<UBulletComponent>(TEXT("BULLET"));
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
		//hard
		auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (Controller->WasInputKeyJustPressed(EKeys::F))
		{
			//detach weapon
			if (OverlappedCharacter->WeaponManager)
			{
				OverlappedCharacter->WeaponManager->ChangeWeapon(this);
			}
		}
	}
}

void AGun::PickUp(AValorantCharacter* Character)
{
	if (Character->WeaponManager)
	{
		Character->WeaponManager->AddWeapon(this);
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
		OverlappedCharacter->WeaponManager->RemoveWeapon(this);
	}
}

int AGun::GetCurrentBullet()
{
	if (BulletComp)
	{
		return BulletComp->GetCurrentBullet();
	}

	return 0;
}

int AGun::GetRemainBullet()
{
	if (BulletComp)
	{
		return BulletComp->GetRemainBullet();
	}

	return 0;
}

int AGun::GetReloadBullet()
{
	if (BulletComp)
	{
		return BulletComp->GetReloadBullet();
	}

	return 0;
}

void AGun::Reload()
{
	if (BulletComp->Reload())
	{
		SetCanFire(true);
		DG_BulletChange.ExecuteIfBound(WeaponTag.ToString());
	}
	else 
	{
		SetCanFire(false);
	}
}

void AGun::DecreaseCurrentBullet()
{

	BulletComp->SetCurrentBullet(BulletComp->GetCurrentBullet() - 1);
	DG_BulletChange.ExecuteIfBound(WeaponTag.ToString());

	//남은 탄이 없으면 장전
	if (BulletComp->GetCurrentBullet() == 0)
	{
		WeaponComp->Reload();
	}
}
