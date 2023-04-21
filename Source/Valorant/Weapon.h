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
	class UTP_WeaponComponent* WeaponComp;

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	class UTP_PickUpComponent* PickUpComp;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	class UTP_InteractionComponent* InteractComp;

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	class UWidgetComponent* InteractUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanInteraction;

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	FName WeaponTag;

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	AValorantCharacter* OverlappedCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PickUp(AValorantCharacter* Character);
	void Interact(AValorantCharacter* Character);
	void EndInteract();
	void DetachWeapon();
	void EnableInteraction();

	UPROPERTY(EditAnywhere)
	int type;

	const int PRIMARY = 0;
	const int SECONDARY = 1;

};
