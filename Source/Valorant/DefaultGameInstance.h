// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "DefaultGameInstance.generated.h"


USTRUCT()
struct FCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CharCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CSkillPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QSkillPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ESkillPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XSkillPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Credit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Shield;
};

USTRUCT()
struct FBullet : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentBullet;

	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 RemainBullet;

	//하나의 탄창에 들어가는 탄 수
	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 ReloadBullet;
};
/**
 * 
 */
UCLASS()
class VALORANT_API UDefaultGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UDefaultGameInstance();

	virtual void Init() override;

	FCharacterStat* GetStatData(FString name);
	FBullet* GetBulletData(FString name);
	USkeletalMesh* GetMesh(FString name);

private:
	UPROPERTY()
	class UDataTable* MyStats;

	UPROPERTY()
	class UDataTable* BulletDT;

	TMap<FString, USkeletalMesh*> WeaponMesh;
};
