// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletComponent.h"
#include "DefaultGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBulletComponent::SetInfo(FString name)
{
	auto MyGameInstance = Cast<UDefaultGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetBulletData(name);
		if (StatData)
		{
			CurrentBullet = StatData->CurrentBullet;
			RemainBullet = StatData->RemainBullet;
			ReloadBullet = StatData->ReloadBullet;
		}
	}
}

// Sets default values for this component's properties
UBulletComponent::UBulletComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBulletComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetInfo(Name);
}


// Called every frame
void UBulletComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBulletComponent::Reload()
{
	if (RemainBullet > 0)
	{
		auto Need = ReloadBullet - CurrentBullet;

		//남은 탄이 필요한 탄보다 적을 때
		if (RemainBullet < Need)
		{
			SetCurrentBullet(RemainBullet);
			SetRemainBullet(0);

			//WidgetBullet->SetCurrentBullet(CurrentBullet);
			//WidgetBullet->SetRemainBullet(RemainBullet);
		}
		else
		{
			//남은 탄이 필요한 탄보다 많을 때
			SetRemainBullet(RemainBullet - Need);
			SetCurrentBullet(RemainBullet);

			//WidgetBullet->SetCurrentBullet(CurrentBullet);
			//WidgetBullet->SetRemainBullet(RemainBullet);
		}
	}

	return CurrentBullet > 0;
}


