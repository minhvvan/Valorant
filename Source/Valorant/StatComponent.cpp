// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "DefaultGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Code = 1;
}

void UStatComponent::SetInfo(int32 code)
{
	auto MyGameInstance = Cast<UDefaultGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(code);
		if (StatData)
		{
			SetHp(StatData->MaxHp);
			MaxHp = StatData->MaxHp;
		}
	}
}

void UStatComponent::SetHp(int32 NewHp)
{
	Hp = NewHp;
	if (Hp < 0) Hp = 0;
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetInfo(Code);
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
