// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BulletComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UBulletComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	void SetInfo(FString name);

	void SetCurrentBullet(int32 point) { CurrentBullet  = point; };
	void SetRemainBullet(int32 point) { RemainBullet = point; };
	void SetReloadBullet(int32 point) { ReloadBullet = point; };

	int32 GetCurrentBullet() { return CurrentBullet; };
	int32 GetRemainBullet() { return RemainBullet; };
	int32 GetReloadBullet() { return ReloadBullet; };
public:	
	// Sets default values for this component's properties
	UBulletComponent();

	bool Reload();

	void SetName(FString name) { Name = name; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentBullet;

	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 RemainBullet;

	//하나의 탄창에 들어가는 탄 수
	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 ReloadBullet;
};
