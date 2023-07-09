// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameInstance.h"

void UDefaultGameInstance::Init()
{
	Super::Init();
}

UDefaultGameInstance::UDefaultGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("/Script/Engine.DataTable'/Game/Data/CharacterStat.CharacterStat'"));
	MyStats = DATA.Object;	
	
	static ConstructorHelpers::FObjectFinder<UDataTable> BULLET(TEXT("/Script/Engine.DataTable'/Game/Data/BulletData.BulletData'"));
	BulletDT = BULLET.Object;

	//vandal
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Vandal(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Weapons/ARAXYS.ARAXYS_GN_AK_Antares_Lv3_Skelmesh_mo'"));
	if (Vandal.Succeeded())
	{
		WeaponMesh.Add(TEXT("Vandal"), Vandal.Object);
	}

	//operator
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Operator(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Weapons/ARAXYS.ARAXYS_GN_Boltsniper_Antares_Lv3_Skelmesh_mo'"));
	if (Operator.Succeeded())
	{
		WeaponMesh.Add(TEXT("Operator"), Operator.Object);
	}

	//BullDog
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BullDog(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Weapons/ARAXYS.ARAXYS_GN_Burst_Antares_Lv3_Skelmesh_mo'"));
	if (BullDog.Succeeded())
	{
		WeaponMesh.Add(TEXT("BullDog"), BullDog.Object);
	}

	//Shorty
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Shorty(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Weapons/ARAXYS.ARAXYS_GN_Slim_Antares_Lv3_Skelmesh_mo'"));
	if (Shorty.Succeeded())
	{
		WeaponMesh.Add(TEXT("Shorty"), Shorty.Object);
	}
}

FCharacterStat* UDefaultGameInstance::GetStatData(FString name)
{
	return MyStats->FindRow<FCharacterStat>(*name, TEXT(""));
}

FBullet* UDefaultGameInstance::GetBulletData(FString name)
{
	return BulletDT->FindRow<FBullet>(*name, TEXT(""));
}

USkeletalMesh* UDefaultGameInstance::GetMesh(FString name)
{
	return *WeaponMesh.Find(name);
}
