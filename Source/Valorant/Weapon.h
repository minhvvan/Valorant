// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ValorantCharacter.h"
#include "Weapon.generated.h"

UCLASS()
class VALORANT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	FName WeaponTag;

	virtual void PickUp(AValorantCharacter* Character) {};
	virtual void Interact(AValorantCharacter* Character) {};
	virtual void EndInteract() {};
	virtual void DetachWeapon() {};
	virtual void EnableInteraction() {};
	virtual void SetCanFire(bool Flag) {};

};
