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

	FCharacterStat* GetStatData(int32 code);

private:
	UPROPERTY()
	class UDataTable* MyStats;
};
