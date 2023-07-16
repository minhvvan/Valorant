// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Market.h"
#include "Components/Button.h"
#include "ValorantCharacter.h"
#include "Weapon.h"
#include "WeaponManager.h"
#include "Gun.h"
#include "TP_PickUpComponent.h"
#include "TP_InteractionComponent.h"


void UWidget_Market::NativeOnInitialized()
{
	ClassicBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickClassicBtn);
	ShortyBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickShortyBtn);
	FrenzyBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickFrenzyBtn);
	GhostBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickGhostBtn);
	SheriffBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickSheriffBtn);
	StingerBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickStingerBtn);
	SpectreBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickSpectreBtn);
	BuckyBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickBuckyBtn);
	JudgeBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickJudgeBtn);
	BulldogBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickBulldogBtn);
	GuardianBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickGuardianBtn);
	VandalBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickVandalBtn);
	PhantomBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickPhantomBtn);
	MarshalBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickMarshalBtn);
	OperatorBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickOperatorBtn);
	AresBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickAresBtn);
	OdinBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickOdinBtn);
	LightShieldBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickLightShieldBtn);
	HeavyShieldBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickHeavyShieldBtn);	
	CSkillBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickCSkillBtn);
	QSkillBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickQSkillBtn);
	ESkillBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickESkillBtn);

	Clicked = FLinearColor(0.f, 1.f, 1.f, .7f);


	WeaponClass = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/FirstPerson/Blueprints/BP_Rifle.BP_Rifle'")));
	//static ConstructorHelpers::FObjectFinder<AWeapon> UW();
	//if (UW.Succeeded())
	//{
	//	WeaponClass = UW.Object;
	//}
}

void UWidget_Market::ClickClassicBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickClassicBtn"));
	ClassicBtn->SetBackgroundColor(Clicked);
}

void UWidget_Market::ClickShortyBtn()
{
	//Shorty 생성 -> 캐릭터 WeaponManager에 추가요청
	FRotator rotator;
	FVector spawnLocation = Character->GetActorLocation() + (10, 10, 10);
	FTransform SpawnTransform(rotator, spawnLocation);
	AWeapon* Shorty = GetWorld()->SpawnActorDeferred<AWeapon>(WeaponClass->GeneratedClass, SpawnTransform);
	if (Shorty)
	{
		Cast<AGun>(Shorty)->SetWeaponName(TEXT("Shorty"));
		Cast<AGun>(Shorty)->PickUpComp->SetGenerateOverlapEvents(false);
		Cast<AGun>(Shorty)->InteractComp->SetGenerateOverlapEvents(false);
		Shorty->WeaponTag = TEXT("Secondary");
		Cast<AGun>(Shorty)->SetMesh(FString(TEXT("Shorty")));

		Shorty->FinishSpawning(SpawnTransform);
		Character->WeaponManager->AddWeapon(Shorty);
	}
	UE_LOG(LogTemp, Warning, TEXT("ClickShortyBtn"));
}

void UWidget_Market::ClickFrenzyBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickFrenzyBtn"));
}

void UWidget_Market::ClickGhostBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickGhostBtn"));
}

void UWidget_Market::ClickSheriffBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickSheriffBtn"));
}

void UWidget_Market::ClickStingerBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickStingerBtn"));
}

void UWidget_Market::ClickSpectreBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickSpectreBtn"));
}

void UWidget_Market::ClickBuckyBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickBuckyBtn"));
}

void UWidget_Market::ClickJudgeBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickJudgeBtn"));
}

void UWidget_Market::ClickBulldogBtn()
{
	FRotator rotator;
	FVector spawnLocation = Character->GetActorLocation() + (10, 10, 10);
	FTransform SpawnTransform(rotator, spawnLocation);
	AWeapon* BullDog = GetWorld()->SpawnActorDeferred<AWeapon>(WeaponClass->GeneratedClass, SpawnTransform);
	if (BullDog)
	{
		Cast<AGun>(BullDog)->SetWeaponName(TEXT("BullDog"));
		Cast<AGun>(BullDog)->PickUpComp->SetGenerateOverlapEvents(false);
		Cast<AGun>(BullDog)->InteractComp->SetGenerateOverlapEvents(false);
		BullDog->WeaponTag = TEXT("Primary");
		Cast<AGun>(BullDog)->SetMesh(FString(TEXT("BullDog")));

		BullDog->FinishSpawning(SpawnTransform);
		Character->WeaponManager->AddWeapon(BullDog);
	}
	UE_LOG(LogTemp, Warning, TEXT("ClickBulldogBtn"));
}

void UWidget_Market::ClickGuardianBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickGuardianBtn"));
}

void UWidget_Market::ClickVandalBtn()
{
	FRotator rotator;
	FVector spawnLocation = Character->GetActorLocation() + (10, 10, 10);
	FTransform SpawnTransform(rotator, spawnLocation);
	AWeapon* Vandal = GetWorld()->SpawnActorDeferred<AWeapon>(WeaponClass->GeneratedClass, SpawnTransform);
	if (Vandal)
	{
		Cast<AGun>(Vandal)->SetWeaponName(TEXT("Vandal"));
		Cast<AGun>(Vandal)->PickUpComp->SetGenerateOverlapEvents(false);
		Cast<AGun>(Vandal)->InteractComp->SetGenerateOverlapEvents(false);
		Vandal->WeaponTag = TEXT("Primary");
		Cast<AGun>(Vandal)->SetMesh(FString(TEXT("Vandal")));

		Vandal->FinishSpawning(SpawnTransform);
		Character->WeaponManager->AddWeapon(Vandal);
	}
	UE_LOG(LogTemp, Warning, TEXT("ClickVandalBtn"));
}

void UWidget_Market::ClickPhantomBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickPhantomBtn"));
}

void UWidget_Market::ClickMarshalBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickMarshalBtn"));
}

void UWidget_Market::ClickOperatorBtn()
{
	FRotator rotator;
	FVector spawnLocation = Character->GetActorLocation() + (10, 10, 10);
	FTransform SpawnTransform(rotator, spawnLocation);
	AWeapon* Operator = GetWorld()->SpawnActorDeferred<AWeapon>(WeaponClass->GeneratedClass, SpawnTransform);
	if (Operator)
	{
		Cast<AGun>(Operator)->SetWeaponName(TEXT("Operator"));
		Cast<AGun>(Operator)->PickUpComp->SetGenerateOverlapEvents(false);
		Cast<AGun>(Operator)->InteractComp->SetGenerateOverlapEvents(false);
		Operator->WeaponTag = TEXT("Primary");
		Cast<AGun>(Operator)->SetMesh(FString(TEXT("Operator")));

		Operator->FinishSpawning(SpawnTransform);
		Character->WeaponManager->AddWeapon(Operator);
	}
	UE_LOG(LogTemp, Warning, TEXT("ClickOperatorBtn"));
}

void UWidget_Market::ClickAresBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickAresBtn"));
}

void UWidget_Market::ClickOdinBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickOdinBtn"));
}

void UWidget_Market::ClickLightShieldBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("Light"));
}

void UWidget_Market::ClickHeavyShieldBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("Heavy"));
}

void UWidget_Market::ClickCSkillBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickCSkillBtn"));
}

void UWidget_Market::ClickQSkillBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickQSkillBtn"));
}

void UWidget_Market::ClickESkillBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickESkillBtn"));
}
