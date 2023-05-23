// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();
	void SetInfo(FString name);
	int32 GetHp() { return Hp; }
	int32 GetMaxHp() { return MaxHp; }
	void SetHp(int32 NewHp);

	void SetCSkillPoint(int32 point) { CSkillPoint = point; };
	void SetQSkillPoint(int32 point) { QSkillPoint = point; };
	void SetESkillPoint(int32 point) { ESkillPoint = point; };
	void SetXSkillPoint(int32 point) { XSkillPoint = point; };

	int32 GetCSkillPoint() { return CSkillPoint; };
	int32 GetQSkillPoint() { return QSkillPoint; };
	int32 GetESkillPoint() { return ESkillPoint; };
	int32 GetXSkillPoint() { return XSkillPoint; };
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	FString Name;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 MaxHp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Hp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 CSkillPoint;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 QSkillPoint;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 ESkillPoint;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 XSkillPoint;
};
