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
	
	//발사
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();	

	//Weapon 소유권 해제
	UFUNCTION()
	void DetachWeapon();

	//발사 가능 여부 설정
	void SetCanFire(bool Flag) { CanFire = Flag; };
protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	virtual void BeginPlay();

private:
	//무기 소유 Character
	AValorantCharacter* Character;

	//최초 Attach시 Input 설정을 위한 변수
	bool Once = true;

	//발사 가능 여부
	bool CanFire = false;

	//무기 소유 Character의 Camera(반동을 위함)
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* PlayerCamera;

	//반동 회복을 위한 Rotation
	FRotator OriginalCameraRotation;

	//반동 Rotation
	FRotator TargetCameraRotation;

	//최대 반동
	float MaxCameraRecoil = 10.f;
	float MaxLeftYaw= -10.f;
	float MaxRightYaw = 10.f;

	//Bullet 반동 Offset
	FVector BulletOffset = {0, 0, 0};

public:
	//반동 적용
	void ApplyCameraRecoil();
	//발사 종료
	void EndFire();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//반동 조정 curve
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun")
	UCurveVector* RecoilStrengthCurve;

	//Bullet반동 Timeline
	FTimeline RecoilOffset;

	//반동 세기
	UPROPERTY(EditAnywhere, Category = "Gun")
	float RecoilStrength;

	//반동 회복 시간(Tick에서 사용)
	UPROPERTY(EditAnywhere, Category = "Gun")
	float RecoilRecoveryTime;

	//사거리
	UPROPERTY(EditAnywhere, Category = "Gun")
	float Range;

	//발사 여부
	bool bFiring = false;

	//반동방향조정을 위한 변수 
	int RemainNum = 0;
	bool RightTurn = false;

	//timeline Callback
	UFUNCTION()
	void OnBulletRecoilProgress(FVector BulletRecoil);

	//Bullet
	int32 CurrentBullet;

	//보유 탄 수 
	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 RemainBullet;

	//하나의 탄창에 들어가는 탄 수
	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 ReloadBullet;

	//장전
	void Reload();

	//탄 Widget
	TSubclassOf<class UBulletWidget> MainHUDWidgetClass;
	class UBulletWidget* WidgetBullet;

	UPROPERTY()
	class UAnimInstance* AnimInstance;

	//AnimMontage Callback(장전 완료)
	UFUNCTION()
	void ReloadAnimEnded(UAnimMontage* Montage, bool bInterrupted);

	//장전 Anim
	const FString ReloadName = "FP_Reload";
};
