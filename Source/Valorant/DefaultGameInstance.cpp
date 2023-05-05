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
}

FCharacterStat* UDefaultGameInstance::GetStatData(FString name)
{
	return MyStats->FindRow<FCharacterStat>(*name, TEXT(""));
}
