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
#include "Spike.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "CollisionQueryParams.h"
#include "BaseGameState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Knife.h"
#include "BaseGameState.h"
#include "StatComponent.h"
#include "Raze_Grenade.h"
#include "Raze_Blast.h"

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

	Stat = CreateDefaultSubobject<UStatComponent>(TEXT("STAT"));

	bCanUnInstall = false;
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
			Subsystem->AddMappingContext(DefaultMappingContext, 1);
			Subsystem->AddMappingContext(SkillMappingContext, 1);
		}
	}
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;
	Knife = GetWorld()->SpawnActor<AKnife>(AKnife::StaticClass(), spawnLocation, rotator, spawnParams);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Knife->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("KnifePoint")));

	CurrentWeapon = Knife;
	auto HP = Stat->GetHp();
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
		
		//Swap
		EnhancedInputComponent->BindAction(SlotOneAction, ETriggerEvent::Triggered, this, &AValorantCharacter::QuickSlotOne);
		EnhancedInputComponent->BindAction(SlotTwoAction, ETriggerEvent::Triggered, this, &AValorantCharacter::QuickSlotTwo);
		EnhancedInputComponent->BindAction(SlotThreeAction, ETriggerEvent::Triggered, this, &AValorantCharacter::QuickSlotThree);
		EnhancedInputComponent->BindAction(SlotFourAction, ETriggerEvent::Triggered, this, &AValorantCharacter::QuickSlotFour);
		EnhancedInputComponent->BindAction(SlotFourAction, ETriggerEvent::Completed, this, &AValorantCharacter::Install);
		EnhancedInputComponent->BindAction(SlotFourAction, ETriggerEvent::Canceled, this, &AValorantCharacter::CancleInstall);
		
		//Drop
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AValorantCharacter::DropCurrentWeapon);

		//Skill
		EnhancedInputComponent->BindAction(CSkillAction, ETriggerEvent::Triggered, this, &AValorantCharacter::SkillC);
		EnhancedInputComponent->BindAction(QSkillAction, ETriggerEvent::Triggered, this, &AValorantCharacter::SkillQ);
		EnhancedInputComponent->BindAction(ESkillAction, ETriggerEvent::Triggered, this, &AValorantCharacter::SkillE);
		EnhancedInputComponent->BindAction(XSkillAction, ETriggerEvent::Triggered, this, &AValorantCharacter::SkillX);
		EnhancedInputComponent->BindAction(SkillActiveAction, ETriggerEvent::Triggered, this, &AValorantCharacter::ActiveSkill);
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
			if (Tag == "Primary")
			{
				SetHasRifle(false);
			}
			else 
			{
				SetHasPistol(false);
			}
			CurrentWeapon = nullptr;

			Weapon->DetachWeapon();
		}
	}
}

void AValorantCharacter::DropWeapon(class AWeapon* Weapon)
{
	FHitResult HisResult;
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() - FVector(0.f, 0.f, 500.f);
	FVector DropPos;
	if (GetWorld()->LineTraceSingleByChannel(HisResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		DropPos = HisResult.Location + FVector(0.f, 0.f, 80.f) + GetActorForwardVector() * 50;
	}
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->SetActorLocation(DropPos);
	Weapon->SetActorRotation(FQuat::Identity);
	Weapon->EnableInteraction();
	Weapon->SetActorHiddenInGame(false);
}

void AValorantCharacter::AddToWeapon(FString Tag, AWeapon* Weapon)
{
	Weapons.Add(Tag, Weapon);
}

void AValorantCharacter::RemoveFromWeapon(FString Tag)
{
	Weapons.FindAndRemoveChecked(Tag);
}

void AValorantCharacter::SetCurrentWeapon(AWeapon* Weapon)
{
	CurrentWeapon = Weapon;
	CurrentWeapon->SetCanFire(true);
	CurrentWeapon->SetActorHiddenInGame(false);
}

float AValorantCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("take Damage: %f"), DamageAmount);
	
	if (Stat)
	{
		auto HP = Stat->GetHp();
		HP -= DamageAmount;
		Stat->SetHp(HP);
	}
	return 0.0f;
}

void AValorantCharacter::Death()
{
	//죽는 Montage Play
	
	//들고있던 무기 떨구기
	//칼은 안떨굼
	if (CurrentWeapon)
	{
		if (!CurrentWeapon->ActorHasTag("Knife"))
		{
			DetachWeapon((CurrentWeapon->WeaponTag).ToString());
		}
	}
	Destroy();
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

void AValorantCharacter::QuickSlotOne(const FInputActionValue& Value)
{
	AWeapon** Container = Weapons.Find("Primary");
	if (Container)
	{
		AWeapon* Weapon = *Container;
		if (CurrentWeapon)
		{
			CurrentWeapon->SetActorHiddenInGame(true);
			CurrentWeapon->SetCanFire(false);
		}
		if (Spike)
		{
			Spike->SetActorHiddenInGame(true);
		}
		if (Knife)
		{
			Knife->SetActorHiddenInGame(true);
		}
		SetCurrentWeapon(Weapon);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pimary None"));
	}
}

void AValorantCharacter::QuickSlotTwo(const FInputActionValue& Value)
{
	AWeapon** Container = Weapons.Find("Secondary");
	if (Container)
	{
		AWeapon* Weapon = *Container;
		if (CurrentWeapon)
		{
			//현재 무기 fire 끄기
			//히든 키기
			CurrentWeapon->SetCanFire(false);
			CurrentWeapon->SetActorHiddenInGame(true);
		}
		if (Spike)
		{
			Spike->SetActorHiddenInGame(true);
		}
		SetCurrentWeapon(Weapon);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Secondary None"));
	}
}

void AValorantCharacter::QuickSlotThree(const FInputActionValue& Value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetCanFire(false);
	}
	if (Spike)
	{
		//CurrentWeapon->SetCanFire(false);
		Spike->SetActorHiddenInGame(true);
	}
	SetCurrentWeapon(Knife);
	Knife->SetCanAttack(true);
	Knife->SetActorHiddenInGame(false);
}

void AValorantCharacter::QuickSlotFour(const FInputActionValue& Value)
{
	if (Spike)
	{
		if (Spike->GetCanInstall())
		{
			UE_LOG(LogTemp, Warning, TEXT("Install Start"));
			OnInstall.Broadcast();
		}
		UE_LOG(LogTemp, Warning, TEXT("Swap"));

		Spike->SetActorHiddenInGame(false);
		if (CurrentWeapon)
		{
			CurrentWeapon->SetCanFire(false);
			CurrentWeapon->SetActorHiddenInGame(true);
		}
	}
	else
	{
		if (bCanUnInstall)
		{
			OnUnInstall.Broadcast();
		}
	}
}

void AValorantCharacter::DropCurrentWeapon(const FInputActionValue& Value)
{
	if (CurrentWeapon)
	{
		auto Tag = CurrentWeapon->WeaponTag.ToString();
		RemoveFromWeapon(Tag);
		DetachWeapon(Tag);
		DropWeapon(CurrentWeapon);
		
		if (Tag == "Primary")
		{
			//주무기 -> 보조무기
			//보조무기 있는지 check
			if (AWeapon** ptr =  Weapons.Find("Secondary"))
			{
				auto temp = *(ptr);
				SetCurrentWeapon(temp);
				temp->SetActorHiddenInGame(false);
				temp->SetCanFire(true);
			}
			else 
			{
				CurrentWeapon = nullptr;
			}
		}
		else
		{
			//보조무기 -> 주무기
			//주무기있나 체크
			if (auto ptr = Weapons.Find("Primary"))
			{
				auto temp = *(ptr);
				SetCurrentWeapon(temp);
				temp->SetActorHiddenInGame(false);
				temp->SetCanFire(true);
			}
			else
			{
				CurrentWeapon = nullptr;
			}
		}
	}
}

void AValorantCharacter::Install(const FInputActionValue& Value)
{
	if (Spike)
	{
		if (Spike->GetCanInstall())
		{
			//설치 완료 -> 타이머 시작
			FDetachWidget.Broadcast();
			OnInstallComplete.Broadcast();

			auto GS = GetWorld()->GetGameState<ABaseGameState>();
			GS->StartTimer();
			//Drop
			{
				FHitResult HisResult;
				FVector Start = GetActorLocation();
				FVector End = GetActorLocation() - FVector(0.f, 0.f, 500.f);
				FVector DropPos;

				FCollisionQueryParams CollisionParameters;
				CollisionParameters.AddIgnoredActor(this);

				if (GetWorld()->LineTraceSingleByChannel(HisResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParameters))
				{
					DropPos = HisResult.Location + GetActorForwardVector() * 20;
				}
				Spike->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				Spike->SetActorLocation(DropPos);
				Spike->SetActorRotation(FQuat::Identity);
				//Spike->EnableInteraction();
				Spike->SetActorHiddenInGame(false);
			}
			Spike->SetCanInstall(false);
			Spike->CanPickUp = false;
			Spike->SetCanInteraction(true);
			Spike = nullptr;
		}
	}
	else
	{
		//스파이크 해제
		if (bCanUnInstall)
		{
			//해제 성공
			FDetachWidget.Broadcast();

			auto GS = GetWorld()->GetGameState<ABaseGameState>();
			GS->Clear();

			OnUnInstallComplete.Broadcast();
		}
	}
}

void AValorantCharacter::CancleInstall(const FInputActionValue& Value)
{
	FDetachWidget.Broadcast();
}

void AValorantCharacter::LiftFail()
{
	UE_LOG(LogTemp, Warning, TEXT("Lift"));
}

void AValorantCharacter::SkillC()
{
}

void AValorantCharacter::SkillQ()
{
	if (Blast)
	{
		Blast->Explosion();
	}
	else
	{
		auto QSP = Stat->QSkillPoint;
		if (QSP > 0)
		{
			Stat->SetQSkillPoint(--QSP);
			if (BlastClass)
			{
				if (auto World = GetWorld())
				{
					if (CurrentWeapon)
					{
						CurrentWeapon->SetActorHiddenInGame(true);
						CurrentWeapon->SetCanFire(false);
					}

					FRotator rotator;
					FVector spawnLocation = GetActorLocation();
					spawnLocation += {1, 1, 1};

					FTransform SpawnTransform(rotator, spawnLocation);
					Blast = GetWorld()->SpawnActorDeferred<ARaze_Blast>(BlastClass, SpawnTransform);
					if (Blast)
					{
						Blast->SetCharacter(this);
						Blast->FinishSpawning(SpawnTransform);

						FVector CameraLocation;
						FRotator CameraRotation;
						GetActorEyesViewPoint(CameraLocation, CameraRotation);

						FRotator MuzzleRotation = CameraRotation;
						MuzzleRotation.Pitch += 10.0f;

						FVector LaunchDirection = MuzzleRotation.Vector();
						Blast->Fire(LaunchDirection);
					}
				}
			}
		}
	}
}

void AValorantCharacter::SkillE()
{
	auto ESP = Stat->ESkillPoint;
	if (ESP > 0)
	{
		Stat->SetESkillPoint(--ESP);
		if (GrenadeClass)
		{
			if (auto World = GetWorld())
			{
				if (CurrentWeapon)
				{
					CurrentWeapon->SetActorHiddenInGame(true);
					CurrentWeapon->SetCanFire(false);
				}

				FRotator rotator;
				FVector spawnLocation = GetActorLocation();
				spawnLocation += {100, 100, 100};

				FTransform SpawnTransform(rotator, spawnLocation);
				Grenade = GetWorld()->SpawnActorDeferred<ARaze_Grenade>(GrenadeClass, SpawnTransform);
				if (Grenade)
				{
					Grenade->SetCharacter(this);
					Grenade->FinishSpawning(SpawnTransform);
					FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
					Grenade->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
				}
			}
		}
	}
}

void AValorantCharacter::SkillX()
{
}

void AValorantCharacter::ActiveSkill()
{
	if (Grenade)
	{
		Grenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		//throw grenade
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		FVector LaunchDirection = MuzzleRotation.Vector();
		Grenade->Fire(LaunchDirection);

		Grenade = nullptr;
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
