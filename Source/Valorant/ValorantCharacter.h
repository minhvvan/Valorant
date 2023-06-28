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

protected:
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* SkillMappingContext;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* QSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ESkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* XSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkillActiveAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon, meta=(AllowPrivateAccess = "true"))
	TMap<FString, class AWeapon*> Weapons;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon, meta=(AllowPrivateAccess = "true"))
	class AWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class ASpike* Spike;

	UPROPERTY(VisibleAnywhere)
	class UStatComponent* Stat;

public:
	UPROPERTY(VisibleAnywhere)
	class UWeaponManager* WeaponManager;
	
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

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasPistol(bool bNewHasRifle);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasPistol();	
	
	//void DetachWeapon(FString Tag);

	//void DropWeapon(class AWeapon* Weapon);

	//UFUNCTION()
	//void AddToWeapon(FString Tag, class AWeapon* Weapon);

	//UFUNCTION()
	//void RemoveFromWeapon(FString Tag);

	UFUNCTION()
	TMap<FString, class AWeapon*> GetWeapons() { return Weapons; };

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

public:
	virtual void SkillC();
	virtual void SkillQ();
	virtual void SkillE();
	virtual void SkillX();

	void ActiveSkill();

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASkillPawn> GrenadeClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASkillPawn> BlastClass;	
	
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASkillPawn> BoomBotClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASkillPawn> ShowStopperClass;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

public:
	class ARaze_Grenade* Grenade;
	class ARaze_Blast* Blast;
	class ARaze_BoomBot* BoomBot;
	class ARaze_Showstopper* ShowStopper;
};

