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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	//Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	//Decal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	UMaterial* ShotHoleMat;

	UTP_WeaponComponent();

	//Weapon -> Character Attach
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AValorantCharacter* TargetCharacter, FString Tag);
	
	//�߻�
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();	

	//Weapon ������ ����
	UFUNCTION()
	void DetachWeapon();

	//�߻� ���� ���� ����
	void SetCanFire(bool Flag) { CanFire = Flag; };
protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	virtual void BeginPlay();

private:
	//���� ���� Character
	AValorantCharacter* Character;

	//���� Attach�� Input ������ ���� ����
	bool Once = true;

	//�߻� ���� ����
	bool CanFire = false;

	//���� ���� Character�� Camera(�ݵ��� ����)
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* PlayerCamera;

	//�ݵ� ȸ���� ���� Rotation
	FRotator OriginalCameraRotation;

	//�ݵ� Rotation
	FRotator TargetCameraRotation;

	//�ִ� �ݵ�
	float MaxCameraRecoil = 10.f;
	float MaxLeftYaw= -10.f;
	float MaxRightYaw = 10.f;

	//Bullet �ݵ� Offset
	FVector BulletOffset = {0, 0, 0};

public:
	//�ݵ� ����
	void ApplyCameraRecoil();
	//�߻� ����
	void EndFire();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//�ݵ� ���� curve
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun")
	UCurveVector* RecoilStrengthCurve;

	//Bullet�ݵ� Timeline
	FTimeline RecoilOffset;

	//�ݵ� ����
	UPROPERTY(EditAnywhere, Category = "Gun")
	float RecoilStrength;

	//�ݵ� ȸ�� �ð�(Tick���� ���)
	UPROPERTY(EditAnywhere, Category = "Gun")
	float RecoilRecoveryTime;

	//��Ÿ�
	UPROPERTY(EditAnywhere, Category = "Gun")
	float Range;

	//�߻� ����
	bool bFiring = false;

	//�ݵ����������� ���� ���� 
	int RemainNum = 0;
	bool RightTurn = false;

	//timeline Callback
	UFUNCTION()
	void OnBulletRecoilProgress(FVector BulletRecoil);

	//Bullet
	int32 CurrentBullet;

	//���� ź �� 
	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 RemainBullet;

	//�ϳ��� źâ�� ���� ź ��
	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 ReloadBullet;

	//����
	void Reload();

	//ź Widget
	TSubclassOf<class UBulletWidget> MainHUDWidgetClass;
	class UBulletWidget* WidgetBullet;

	UPROPERTY()
	class UAnimInstance* AnimInstance;

	//AnimMontage Callback(���� �Ϸ�)
	UFUNCTION()
	void ReloadAnimEnded(UAnimMontage* Montage, bool bInterrupted);

	//���� Anim
	const FString ReloadName = "FP_Reload";
};
