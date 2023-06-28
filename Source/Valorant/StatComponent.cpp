// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "DefaultGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ValorantCharacter.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	//Name = "Sage";
}

void UStatComponent::SetInfo(FString name)
{
	auto MyGameInstance = Cast<UDefaultGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(name);
		if (StatData)
		{
			SetHp(StatData->MaxHp);
			MaxHp = StatData->MaxHp;
			CSkillPoint = StatData->CSkillPoint;
			QSkillPoint = StatData->QSkillPoint;
			ESkillPoint = StatData->ESkillPoint;
			XSkillPoint = StatData->XSkillPoint;
		}
	}
}

void UStatComponent::SetHp(int32 NewHp)
{
	Hp = NewHp;
	if (Hp <= 0)
	{
		Hp = 0;
		auto Player = Cast<AValorantCharacter>(GetOwner());
		Player->Death();
	}
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetInfo(Name);
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

