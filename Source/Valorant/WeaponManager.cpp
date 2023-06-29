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

	// knife생성 후 character에 세팅
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

//Weapon을 받아 map에 저장
bool UWeaponManager::AddWeapon(AWeapon* Weapon)
{
	auto Tag = Weapon->WeaponTag.ToString();

	//이미 같은 종류의 무기가 있으면 return
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

	//무기 종류에 따라 설정 -> Anim 관련 변수
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
	//자료구조에서 제거
	auto Tag = Weapon->WeaponTag.ToString();
	Weapons.FindAndRemoveChecked(Tag);

	//world에 떨구기
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

	//발사 불가 & 소유권 해제
	Weapon->SetCanFire(false);
	//Weapon->Once = true;

		//무기 종류에 따라 설정 -> Anim 관련 변수
	if (Tag == "Primary")
	{
		Character->SetHasRifle(false);
	}
	else
	{
		Character->SetHasPistol(false);
	}

	// 주 -> 보 -> 칼 로 우선순위대로 바뀐다
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

	//world에 떨구기
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

		//발사 불가 & 소유권 해제
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

		//탄 Widget 설정
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
					// 위젯을 뷰포트에 띄우는 함수
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
