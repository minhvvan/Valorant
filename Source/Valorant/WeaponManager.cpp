// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include "ValorantCharacter.h"
#include "Weapon.h"
#include "Knife.h"
#include "TP_WeaponComponent.h"
#include "TP_InteractionComponent.h"
#include "Gun.h"
#include "BulletWidget.h"

// Sets default values for this component's properties
UWeaponManager::UWeaponManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widget/WBP_Bullet.WBP_Bullet_C'"));
	if (UW.Succeeded())
	{
		MainHUDWidgetClass = UW.Class;
	}
	// ...
}


// Called when the game starts
void UWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AValorantCharacter>(GetOwner());

	// knife���� �� character�� ����
	//FActorSpawnParameters spawnParams;
	//FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;
	//auto Knife = GetWorld()->SpawnActor<AKnife>(AKnife::StaticClass(), spawnLocation, rotator, spawnParams);
	
	FRotator rotator;
	FTransform SpawnTransform(rotator, spawnLocation);
	AWeapon* Knife = GetWorld()->SpawnActorDeferred<AWeapon>(KnifeClass, SpawnTransform);
	if (Knife)
	{
		Knife->FinishSpawning(SpawnTransform);
		Character->SetCurrentWeapon(Knife);
		AddWeapon(Knife);
	}
}


// Called every frame
void UWeaponManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//Weapon�� �޾� map�� ����
bool UWeaponManager::AddWeapon(AWeapon* Weapon)
{
	auto Tag = Weapon->WeaponTag.ToString();

	//�̹� ���� ������ ���Ⱑ ������ return
	if(Weapons.Contains(Tag))
	{
		return false;
	}

	Weapons.Add(Tag, Weapon);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Weapon->AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	Weapon->SetActorHiddenInGame(true);

	auto Gun = Cast<AGun>(Weapon);
	if (Gun && Gun->WeaponComp)
	{
		Gun->DG_BulletChange.BindUObject(this, &UWeaponManager::SetBulletWidget);

		Gun->WeaponComp->AttachWeapon(Character, Tag);
		Gun->InteractComp->SetGenerateOverlapEvents(false);
	}

	//���� ������ ���� ���� -> Anim ���� ����
	//if (Tag == "Primary")
	//{
	//	Character->SetHasRifle(true);
	//}
	//else if(Tag == "Secondary")
	//{
	//	Character->SetHasPistol(true);
	//}

	return true;
}

void UWeaponManager::RemoveWeapon(AWeapon* Weapon)
{
	//�ڷᱸ������ ����
	auto Tag = Weapon->WeaponTag.ToString();
	Weapons.FindAndRemoveChecked(Tag);

	//world�� ������
	FHitResult HisResult;
	FVector Start = Character->GetActorLocation();
	FVector End = Character->GetActorLocation() - FVector(0.f, 0.f, 500.f);
	FVector DropPos;
	if (GetWorld()->LineTraceSingleByChannel(HisResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		DropPos = HisResult.Location + FVector(0.f, 0.f, 80.f) + Character->GetActorForwardVector() * 50;
	}

	if (Weapon)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->SetActorLocation(DropPos);
		Weapon->SetActorRotation(FQuat::Identity);
		Weapon->EnableInteraction();
		Weapon->SetActorHiddenInGame(false);
	}

	//�߻� �Ұ� & ������ ����
	Weapon->SetCanFire(false);
	//Weapon->Once = true;

		//���� ������ ���� ���� -> Anim ���� ����
	if (Tag == "Primary")
	{
		Character->SetHasRifle(false);
	}
	else
	{
		Character->SetHasPistol(false);
	}

	// �� -> �� -> Į �� �켱������� �ٲ��
	{
		auto Container = Weapons.Find("Primary");
		if (Container)
		{
			if (*Container != nullptr)
			{
				Character->SetCurrentWeapon(*Container);
				return;
			}
		}
	}
	{
		auto Container = Weapons.Find("Secondary");
		if (Container)
		{
			if (*Container != nullptr)
			{
				Character->SetCurrentWeapon(*Container);
				return;
			}
		}
	}
	{
		auto Container = Weapons.Find("Knife");
		if (Container)
		{
			if (*Container != nullptr)
			{
				Character->SetCurrentWeapon(*Container);
				return;
			}
		}
	}
}

void UWeaponManager::ChangeWeapon(AWeapon* Weapon)
{
	auto Tag = Weapon->WeaponTag.ToString();
	auto DropWeapon = Weapons.FindAndRemoveChecked(Tag);

	//world�� ������
	FHitResult HisResult;
	FVector Start = Character->GetActorLocation();
	FVector End = Character->GetActorLocation() - FVector(0.f, 0.f, 500.f);
	FVector DropPos;
	if (GetWorld()->LineTraceSingleByChannel(HisResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		DropPos = HisResult.Location + FVector(0.f, 0.f, 80.f) + Character->GetActorForwardVector() * 50;
	}

	if (DropWeapon)
	{
		DropWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		DropWeapon->SetActorLocation(DropPos);
		DropWeapon->SetActorRotation(FQuat::Identity);
		DropWeapon->EnableInteraction();
		DropWeapon->SetActorHiddenInGame(false);

		//�߻� �Ұ� & ������ ����
		DropWeapon->SetCanFire(false);
		AddWeapon(Weapon);
	}
}

void UWeaponManager::SwapWeapon(FString Tag)
{
	auto weaponContainer = Weapons.Find(Tag);

	if (weaponContainer)
	{
		auto weapon = *weaponContainer;
		if (weapon != nullptr)
		{
			Character->GetCurrentWeapon()->SetActorHiddenInGame(true);
			Character->GetCurrentWeapon()->SetCanFire(false);

			Character->SetCurrentWeapon(weapon);
			SetBulletWidget(Tag);

			if (Tag == "Primary")
			{
				Character->SetHasRifle(true);
			}
			else if(Tag == "Secondary")
			{
				Character->SetHasPistol(true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon None"));
		}
	}
}

void UWeaponManager::SetBulletWidget(FString Tag)
{
	auto Weapon = Cast<AGun>(*Weapons.Find(Tag));
	if (Weapon)
	{
		auto CurrentBullet = Weapon->GetCurrentBullet();
		auto ReloadBullet = Weapon->GetReloadBullet();
		auto RemainBullet = Weapon->GetRemainBullet();

		//ź Widget ����
		if (WidgetBullet)
		{
			WidgetBullet->SetVisibility(ESlateVisibility::Visible);

			WidgetBullet->SetCurrentBullet(CurrentBullet);
			WidgetBullet->SetRemainBullet(RemainBullet);
		}
		else {
			if (MainHUDWidgetClass)
			{
				WidgetBullet = Cast<UBulletWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));
				if (IsValid(WidgetBullet))
				{
					// ������ ����Ʈ�� ���� �Լ�
					CurrentBullet = ReloadBullet;
					WidgetBullet->AddToViewport();
					WidgetBullet->SetCurrentBullet(CurrentBullet);
					WidgetBullet->SetRemainBullet(RemainBullet);
				}
			}
		}
	}
	else
	{
		//Knife
		if (WidgetBullet)
		{
			WidgetBullet->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
