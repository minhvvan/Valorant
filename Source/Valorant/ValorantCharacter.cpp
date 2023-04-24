// Copyright Epic Games, Inc. All Rights Reserved.

#include "ValorantCharacter.h"
#include "ValorantProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapon.h"
#include "TP_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// AValorantCharacter

AValorantCharacter::AValorantCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	bHasPistol = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	CurrentWeapon = nullptr;
}

void AValorantCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AValorantCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AValorantCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AValorantCharacter::Look);
	}
}

void AValorantCharacter::DetachWeapon(FString Tag)
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto* Attached : AttachedActors)
	{
		auto Weapon = Cast<AWeapon>(Attached);
		if (Weapon->ActorHasTag(FName(*Tag)))
		{
			Weapon->DetachWeapon();

			FHitResult HisResult;
			FVector Start = GetActorLocation();
			FVector End = GetActorLocation() - FVector(0.f, 0.f, 500.f);
			FVector DropPos;
			if (GetWorld()->LineTraceSingleByChannel(HisResult, Start, End, ECollisionChannel::ECC_Visibility))
			{
				DropPos = HisResult.Location + FVector(0.f, 0.f, 80.f) + GetActorForwardVector() * 50;
			}
			Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			Weapon->SetActorLocation(DropPos);
			Weapon->EnableInteraction();
			Weapon->SetActorHiddenInGame(false);

			//~TODO: 이러면 현재무기 바뀜(보조무기 -> 주무기 이렇게) & Fire가 이상함
			UE_LOG(LogTemp, Warning, TEXT("Detach: %s"), *DropPos.ToString());
		}
	}
}

void AValorantCharacter::AddToWeapon(FString Tag, AWeapon* Weapon)
{
	Weapons.Add(Tag, Weapon);
}

void AValorantCharacter::RemoveFromWeapon(FString Tag)
{
	Weapons.FindAndRemoveChecked(Tag);
}

void AValorantCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AValorantCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AValorantCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AValorantCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AValorantCharacter::SetHasPistol(bool bNewHasPistol)
{
	bHasPistol = bNewHasPistol;
}

bool AValorantCharacter::GetHasPistol()
{
	return bHasPistol;
}
