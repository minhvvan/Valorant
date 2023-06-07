// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillPawn.h"
#include "Raze_Showstopper.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ARaze_Showstopper : public ASkillPawn
{
	GENERATED_BODY()
public:
	ARaze_Showstopper();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	virtual void Fire(FVector Direction) override;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float PendingTime = 5.f;

	FTimerHandle ExplosionTimerHandle;

	FVector MuzzleOffset;

	void Expire();

	class AValorantCharacter* Character;

	UPROPERTY(EditAnywhere)
	float RecoilStrength = 100.f;

	void SetCharacter(class AValorantCharacter* owner) { Character = owner; };

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASkillPawn> RocketClass;
protected:
	void BeginPlay() override;
};
