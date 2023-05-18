// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "TP_WeaponComponent.generated.h"

class AValorantCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AValorantProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	UAnimMontage* ReloadAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	UMaterial* ShotHoleMat;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AValorantCharacter* TargetCharacter, FString Tag);
	
	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();	

	UFUNCTION()
	void DetachWeapon();

	void SetCanFire(bool Flag) { CanFire = Flag; };
protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	virtual void BeginPlay();

private:
	/** The Character holding this weapon*/
	AValorantCharacter* Character;

	bool Once = true;
	bool CanFire = false;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* PlayerCamera;

	FRotator OriginalCameraRotation;
	FRotator TargetCameraRotation;
	float MaxCameraRecoil = 10.f;
	float MaxLeftYaw= -10.f;
	float MaxRightYaw = 10.f;

	float TimelineCursor;
	FVector BulletOffset = {0, 0, 0};

public:
	void ApplyCameraRecoil();
	void EndFire();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun")
	UCurveVector* RecoilStrengthCurve;

	FTimeline RecoilOffset;

	UPROPERTY(EditAnywhere, Category = "Gun")
	float RecoilStrength;

	UPROPERTY(EditAnywhere, Category = "Gun")
	float RecoilRecoveryTime;

	UPROPERTY(EditAnywhere, Category = "Gun")
	float Range;

	bool bFiring = false;

	int RemainNum = 0;

	bool RightTurn = false;

	UFUNCTION()
	void OnBulletRecoilProgress(FVector BulletRecoil);

	//Bullet
	int32 CurrentBullet;

	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 RemainBullet;

	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 ReloadBullet;

	void Reload();

	TSubclassOf<class UBulletWidget> MainHUDWidgetClass;
	class UBulletWidget* WidgetBullet;

	UPROPERTY()
	class UAnimInstance* AnimInstance;

	UFUNCTION()
	void ReloadAnimEnded(UAnimMontage* Montage, bool bInterrupted);

	const FString ReloadName = "FP_Reload";
};
