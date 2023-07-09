// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Gun.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnCanged, FString);
UCLASS()
class VALORANT_API AGun : public AWeapon
{
	GENERATED_BODY()

public:
	AGun();

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
	AValorantCharacter* OverlappedCharacter;

	UPROPERTY(VisibleAnywhere)
	class UBulletComponent* BulletComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PickUp(AValorantCharacter* Character) override;
	virtual void Interact(AValorantCharacter* Character) override;
	virtual void EndInteract() override;
	//virtual  void DetachWeapon() override;
	virtual void EnableInteraction() override;
	virtual void SetCanFire(bool Flag) override;

	UFUNCTION()
	void Reload();
	
	UFUNCTION()
	void DecreaseCurrentBullet();

	void Drop();

	int GetCurrentBullet();
	int GetRemainBullet();
	int GetReloadBullet();

	void SetMesh(FString Name);

	class UDefaultGameInstance* MyGameInstance;

	FOnCanged DG_BulletChange;
};
