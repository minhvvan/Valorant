// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ValorantCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInstall);

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AValorantCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere)
	class AKnife* Knife;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SlotOneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SlotTwoAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SlotThreeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SlotFourAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon, meta=(AllowPrivateAccess = "true"))
	TMap<FString, class AWeapon*> Weapons;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon, meta=(AllowPrivateAccess = "true"))
	class AWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class ASpike* Spike;

	UPROPERTY(VisibleAnywhere)
	class UStatComponent* Stat;

public:
	AValorantCharacter();

	UPROPERTY(BlueprintAssignable)
	FOnInstall OnInstall;

	UPROPERTY(BlueprintAssignable)
	FOnInstall OnInstallComplete;	
	
	UPROPERTY(BlueprintAssignable)
	FOnInstall FDetachWidget;

	UPROPERTY(BlueprintAssignable)
	FOnInstall OnUnInstall;

	UPROPERTY(BlueprintAssignable)
	FOnInstall OnUnInstallComplete;

protected:
	virtual void BeginPlay();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasPistol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanUnInstall;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasPistol(bool bNewHasRifle);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasPistol();	
	
	void DetachWeapon(FString Tag);

	UFUNCTION()
	void AddToWeapon(FString Tag, class AWeapon* Weapon);

	UFUNCTION()
	void RemoveFromWeapon(FString Tag);

	UFUNCTION()
	class AWeapon* GetCurrentWeapon() { return CurrentWeapon; };

	UFUNCTION()
	void SetCurrentWeapon(class AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	class ASpike* GetSpike() { return Spike; };

	UFUNCTION()
	void SetSpike(class ASpike* S) { Spike = S; };

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Death();
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for swap  input */
	void QuickSlotOne(const FInputActionValue& Value);
	void QuickSlotTwo(const FInputActionValue& Value);
	void QuickSlotThree(const FInputActionValue& Value);
	void QuickSlotFour(const FInputActionValue& Value);
	void DropCurrentWeapon(const FInputActionValue& Value);

	void Install(const FInputActionValue& Value);
	void CancleInstall(const FInputActionValue& Value);

	void LiftFail();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	
};

