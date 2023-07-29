// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffects.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UStatusEffects : public UObject
{
	GENERATED_BODY()

public:
	UStatusEffects();

	virtual void Fire() {};

	void SetTime(float t) { time = t; };
	void SetRange(float r) { range = r; };
	void SetOwner(AActor* owner) { Owner = owner; };

protected:
	float time;
	float range;
	AActor* Owner;
};

UCLASS()
class VALORANT_API USEFlash : public UStatusEffects
{
	GENERATED_BODY()

public:
	USEFlash();

	void Fire() override;

private:
	const float FOV = 90.f;

	TSubclassOf<class UWidget_Flash> FlashClass;
	class UWidget_Flash* FlashWidget;
};

UCLASS()
class VALORANT_API USEConcussion : public UStatusEffects
{
	GENERATED_BODY()

public:
	USEConcussion();

	virtual void Fire() override;
};