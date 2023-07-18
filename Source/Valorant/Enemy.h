// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValorantCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class VALORANT_API AEnemy : public AValorantCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = AI)
	float searchRadius;

	UPROPERTY(EditAnywhere, Category = AI)
	float FOV;

	UBlueprint* WeaponClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = AI)
	float GetSearchRadius() { return searchRadius; };

	UFUNCTION(BlueprintCallable, Category = AI)
	float GetFOV() { return FOV; };
};
