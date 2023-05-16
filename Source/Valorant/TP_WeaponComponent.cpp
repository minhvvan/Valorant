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

	//Line Trage�� Hit ����
	auto Player = Cast<AValorantCharacter>(GetOwner()->GetAttachParentActor());
	auto Camera = Player->GetFirstPersonCameraComponent();
	FVector CameraLoc = Camera->GetComponentLocation();
	FVector CameraForward = Camera->GetForwardVector();
	FVector StartLoc = CameraLoc; // ������ ���� ����.
	FVector EndLoc = CameraLoc + (CameraForward * 5000.0f); // ������ ������ ����.

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ��Ʈ ������ ������Ʈ ������.
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(WorldStatic);
	ObjectTypes.Add(WorldDynamic);
	ObjectTypes.Add(Pawn);

	TArray<AActor*> IgnoreActors; // ������ ���͵�.
	IgnoreActors.Add(GetOwner());
	IgnoreActors.Add(Player);
	auto Weapons = Player->GetWeapons();
	for (auto& weapon : Weapons)
	{
		IgnoreActors.Add(weapon.Value);
	}

	FHitResult HitResult; // ��Ʈ ��� �� ���� ����.

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors, // ������ ���� ���ٰ��ص� null�� ���� �� ����.
		EDrawDebugTrace::None,
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
			//���� �� �����µ� ���� ȸ���� �ȵż� �̻��ϰ� ����
			decal->SetDecalMaterial(ShotHoleMat);
			decal->SetLifeSpan(2.0f);
			decal->GetDecal()->DecalSize = FVector(5.0f, 5.0f, 5.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
		}

		if (PlayerCamera)
		{
			OriginalCameraRotation = PlayerCamera->GetComponentRotation();

			TargetCameraRotation.Pitch += RecoilStrength;
			if (TargetCameraRotation.Pitch > MaxCameraRecoil)
			{
				TargetCameraRotation.Pitch = MaxCameraRecoil;
			}
		}

		ApplyCameraRecoil();

		auto victim = HitResult.GetActor();
		UGameplayStatics::ApplyDamage(victim, 10, Player->GetController(), Player, NULL);
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

	//!�̹� ������ ���Ⱑ ����
	AWeapon* Weapon = Cast<AWeapon>(GetOwner());
	if (auto Current = Character->GetCurrentWeapon())
	{
		//���繫�Ⱑ Į�̸�
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
			//���� ���Ⱑ �ٲ��
			if (Character->GetCurrentWeapon()->ActorHasTag(FName(*Tag)))
			{
				CanFire = true;
			}
			else
			{
				//���� ����� �ٸ� ����
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
}

void UTP_WeaponComponent::ApplyCameraRecoil()
{
	if (PlayerCamera)
	{
		FRotator NewCameraRotation = OriginalCameraRotation + TargetCameraRotation;
		
		// ī�޶� ������Ʈ�� ȸ���� ����
		PlayerCamera->SetWorldRotation(NewCameraRotation);
	}
}

void UTP_WeaponComponent::EndFire()
{
	CurrentRecoveryTime = RecoilRecoveryTime;
	UE_LOG(LogTemp, Warning, TEXT("EndFire"));
}


void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentRecoveryTime -= DeltaTime;
	if (PlayerCamera)
	{
		if (0.f <= CurrentRecoveryTime)
		{
			UE_LOG(LogTemp, Warning, TEXT("Recover"));
			FRotator SmoothedRotation = FMath::RInterpTo(PlayerCamera->GetComponentRotation(), OriginalCameraRotation, GetWorld()->GetDeltaSeconds(), 10.f);
			PlayerCamera->SetWorldRotation(SmoothedRotation);
		}
	}
}
