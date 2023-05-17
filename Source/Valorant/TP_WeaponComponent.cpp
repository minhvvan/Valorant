// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "ValorantCharacter.h"
#include "ValorantProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapon.h"
#include "ValorantCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Math/Vector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Resources/IMG/ShotHole_Mat.ShotHole_Mat'"));

	if (Material.Object != NULL)
	{
		ShotHoleMat = (UMaterial*)Material.Object;
	}
}

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr || !CanFire)
	{
		return;
	}

	//~~Camera Recoil
	if (PlayerCamera)
	{
		if (!bFiring)
		{
			OriginalCameraRotation = PlayerCamera->GetRelativeRotation();
			bFiring = true;
		}

		auto randomRecoil = FMath::RandRange(0.5, 0.8);
		//~위쪽 반동
		if (TargetCameraRotation.Pitch < MaxCameraRecoil)
		{
			TargetCameraRotation.Pitch += (RecoilStrength * randomRecoil);
		}
		else
		{
			//~좌우 반동
			if (RemainNum > 0)
			{
				RemainNum--;
			}
			else if (RemainNum == 0)
			{
				RightTurn = !RightTurn;
				RemainNum = FMath::RandRange(5, 13);
			}

			if (RightTurn)
			{
				TargetCameraRotation.Yaw += (RecoilStrength * randomRecoil);
				if (TargetCameraRotation.Yaw > MaxRightYaw)
				{
					TargetCameraRotation.Yaw = MaxRightYaw;
				}
			}
			else
			{
				TargetCameraRotation.Yaw -= (RecoilStrength * randomRecoil);
				if (TargetCameraRotation.Yaw < MaxLeftYaw)
				{
					TargetCameraRotation.Yaw = MaxLeftYaw;
				}
			}
		}
		ApplyCameraRecoil();
	}

	//~HIt Check & Bullet Recoil
	{
		FVector CameraLoc = PlayerCamera->GetComponentLocation();
		FVector CameraForward = PlayerCamera->GetForwardVector();
		FVector StartLoc = CameraLoc;
		UE_LOG(LogTemp, Warning, TEXT("Fire:%s"), *FString(BulletOffset.ToString()));
		FVector EndLoc = CameraLoc + ((CameraForward + BulletOffset) * Range);

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
		TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
		TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
		TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
		ObjectTypes.Add(WorldStatic);
		ObjectTypes.Add(WorldDynamic);
		ObjectTypes.Add(Pawn);

		TArray<AActor*> IgnoreActors; // 무시할 액터들.
		IgnoreActors.Add(GetOwner());
		IgnoreActors.Add(Character);
		auto Weapons = Character->GetWeapons();
		for (auto& weapon : Weapons)
		{
			IgnoreActors.Add(weapon.Value);
		}

		FHitResult HitResult;

		bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			StartLoc,
			EndLoc,
			ObjectTypes,
			false,
			IgnoreActors,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true
		);

		if (Result == true)
		{
			FVector ImpactPoint = HitResult.ImpactPoint;
			FVector impactNormal = HitResult.ImpactNormal;

			FVector basis = FVector(0, 0, 1);
			if (fabsf(impactNormal.Y) > 0.8) {
				basis = FVector(1, 0, 1);
			}
			FVector right = FVector::CrossProduct(impactNormal, basis).GetUnsafeNormal();
			FVector forward = FVector::CrossProduct(right, impactNormal);
			FBasisVectorMatrix bvm(forward, right, impactNormal, FVector(0, 0, 0));
			FRotator theRotation = bvm.Rotator();

			//Decal
			ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(ImpactPoint, theRotation);
			if (decal)
			{
				//땅은 잘 나오는데 벽은 회전이 안돼서 이상하게 나옴
				decal->SetDecalMaterial(ShotHoleMat);
				decal->SetLifeSpan(2.0f);
				decal->GetDecal()->DecalSize = FVector(5.0f, 5.0f, 5.0f);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
			}

			auto victim = HitResult.GetActor();
			UGameplayStatics::ApplyDamage(victim, 10, Character->GetController(), Character, NULL);
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::AttachWeapon(AValorantCharacter* TargetCharacter, FString Tag)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	PlayerCamera = Character->GetFirstPersonCameraComponent();

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	//!이미 장착한 무기가 없음
	AWeapon* Weapon = Cast<AWeapon>(GetOwner());
	if (auto Current = Character->GetCurrentWeapon())
	{
		//현재무기가 칼이면
		if (Current->WeaponTag == "Knife")
		{
			CanFire = true;
			if (Weapon)
			{
				Character->SetCurrentWeapon(Weapon);
			}
		}
		else
		{
			//현재 무기가 바뀐것
			if (Character->GetCurrentWeapon()->ActorHasTag(FName(*Tag)))
			{
				CanFire = true;
			}
			else
			{
				//현재 무기랑 다른 무기
				CanFire = false;
				Weapon->SetActorHiddenInGame(true);
			}
		}
	}

	if (Tag == "Primary")
	{
		Character->SetHasRifle(true);
	}
	else
	{
		Character->SetHasPistol(true); 
	}

	if (Once)
	{
		if(APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
				Subsystem->AddMappingContext(FireMappingContext, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// Fire
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Canceled, this, &UTP_WeaponComponent::EndFire);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::EndFire);
			}
			Once = false;
		}
	}
}

void UTP_WeaponComponent::DetachWeapon()
{
	CanFire = false;
	Character = nullptr;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (RecoilStrengthCurve)
	{
		FOnTimelineVector CurveCallbak;
		CurveCallbak.BindUFunction(this, FName("OnBulletRecoilProgress"));

		RecoilOffset.AddInterpVector(RecoilStrengthCurve, CurveCallbak);
		RecoilOffset.SetLooping(true);

		RecoilOffset.Play();
	}
}

void UTP_WeaponComponent::ApplyCameraRecoil()
{
	if (PlayerCamera)
	{
		FRotator NewCameraRotation = OriginalCameraRotation + TargetCameraRotation;
		PlayerCamera->SetRelativeRotation(NewCameraRotation);
	}
}

void UTP_WeaponComponent::EndFire()
{
	TargetCameraRotation = FRotator::ZeroRotator;

	bFiring = false;
	RemainNum = 0;
	RightTurn = false;
	RecoilOffset.SetPlaybackPosition(0, false);
	BulletOffset = FVector::ZeroVector;
}


void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bFiring)
	{
		RecoilOffset.TickTimeline(DeltaTime);
	}

	if (PlayerCamera)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(TargetCameraRotation.ToString()));

		if (!PlayerCamera->GetRelativeRotation().IsNearlyZero(0.1))
		{
			FRotator SmoothedRotation = FMath::RInterpTo(PlayerCamera->GetRelativeRotation(), FRotator::ZeroRotator, GetWorld()->GetDeltaSeconds(), RecoilRecoveryTime);
			PlayerCamera->SetRelativeRotation(SmoothedRotation);
		}
		else 
		{
			PlayerCamera->SetRelativeRotation(FRotator::ZeroRotator);
		}
	}
}

void UTP_WeaponComponent::OnBulletRecoilProgress(FVector BulletRecoil)
{
	//Z: 상하, Y: 죄우
	auto tempZ = FMath::RandRange(-.01f, .03f);
	auto tempY = FMath::RandRange(-.1f, .3f);
	BulletOffset = { 0, tempY, tempZ };

	BulletOffset *= BulletRecoil;
}
