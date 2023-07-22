// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "ValorantCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Math/Vector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DefaultAnimInstance.h"
#include "BulletWidget.h"
#include "Gun.h"
#include "WeaponManager.h"
#include "DefaultGameInstance.h"
#include "DrawDebugHelpers.h"

UTP_WeaponComponent::UTP_WeaponComponent()
{
	MuzzleOffset = FVector(30.0f, 0.0f, 0.0f);

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Resources/IMG/ShotHole_Mat.ShotHole_Mat'"));

	if (Material.Object != NULL)
	{
		ShotHoleMat = (UMaterial*)Material.Object;
	}

	WeaponName = TEXT("None");
}

//�߻�
void UTP_WeaponComponent::Fire()
{
	//Attach�� Character�� ������ return
	if (Character == nullptr || Character->GetController() == nullptr || !CanFire)
	{
		return;
	}

	//~~Camera Recoil
	if (PlayerCamera)
	{
		//��ź
		if (!bFiring)
		{
			//�������� ȸ���� ��ġ ����
			//OriginalCameraRotation = PlayerCamera->GetRelativeRotation();
			if (Character)
			{
				//OriginalCameraRotation = Character->GetControlRotation();
				bFiring = true;
			}
		}

		//������ �ݵ� ����
		auto randomRecoil = FMath::RandRange(0.5, 0.8);
		//~���� �ݵ�
		if (RecoilRotation.Pitch < MaxCameraRecoil)
		{
			RecoilRotation.Pitch += (RecoilStrength * randomRecoil);
		}
		else
		{
			//~�¿� �ݵ�
			//������ �ٲٱ���� ���� ź ��
			if (RemainNum > 0)
			{
				RemainNum--;
			}
			//���� ��ȯ
			else if (RemainNum == 0)
			{
				RightTurn = !RightTurn;
				RemainNum = FMath::RandRange(5, 13);
			}

			//���� �ݵ�
			if (RightTurn)
			{
				RecoilRotation.Yaw += (RecoilStrength * randomRecoil);
				if (RecoilRotation.Yaw > MaxRightYaw)
				{
					RecoilRotation.Yaw = MaxRightYaw;
				}
			}
			//���� �ݵ�
			else
			{
				RecoilRotation.Yaw -= (RecoilStrength * randomRecoil);
				if (RecoilRotation.Yaw < MaxLeftYaw)
				{
					RecoilRotation.Yaw = MaxLeftYaw;
				}
			}
		}

		//~HIt Check & Bullet Recoil
		{
			//Hit ���� ����
			FVector CameraLocation;
			FRotator CameraRotation;

			CameraLocation = PlayerCamera->GetComponentLocation();
			CameraRotation = PlayerCamera->GetComponentRotation();

			FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
			FVector CameraForward = PlayerCamera->GetForwardVector();
			FVector StartLoc = MuzzleLocation;
			FVector EndLoc = StartLoc + ((CameraForward + BulletOffset) * Range);

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ��Ʈ ������ ������Ʈ ������.
			TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
			TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
			TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
			ObjectTypes.Add(WorldStatic);
			ObjectTypes.Add(WorldDynamic);
			ObjectTypes.Add(Pawn);

			TArray<AActor*> IgnoreActors; // ������ ���͵�.
			IgnoreActors.Add(GetOwner());
			IgnoreActors.Add(Character);
			auto Weapons = Character->WeaponManager->GetWeapons();
			for (auto& weapon : Weapons)
			{
				IgnoreActors.Add(weapon.Value);
			}

			FHitResult HitResult;

			//��Ʈ ����
			bool Result = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartLoc,
				EndLoc,
				ECC_GameTraceChannel4
			);

			DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Emerald, true, 2.f, 0, .3f);

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
					decal->SetDecalMaterial(ShotHoleMat);
					decal->SetLifeSpan(2.0f);
					decal->GetDecal()->DecalSize = FVector(5.0f, 5.0f, 5.0f);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
				}

				//�ǰݵ� Actor Damage����
				auto victim = HitResult.GetActor();
				auto comp = HitResult.GetComponent();

				//comp->GetName()
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *(comp->GetName()));
				float damage = DamageTable.FindRef(comp->GetName());
				UGameplayStatics::ApplyDamage(victim, damage, Character->GetController(), Character, NULL);
			}
		}

		//�ݵ� ����
		ApplyCameraRecoil();
	}

	//�Ҹ� Play
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	//Anim Play
	if (FireAnimation != nullptr)
	{
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	//���� ź �� ����
	auto Owner = Cast<AGun>(GetOwner());
	if (Owner)
	{
		Owner->DecreaseCurrentBullet();
	}
}

//Actor���� ���� ������ 
void UTP_WeaponComponent::AttachWeapon(AValorantCharacter* TargetCharacter, FString Tag)
{

	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	//���� Anim Callback ����
	AnimInstance = Cast<UAnimInstance>(Character->GetMesh1P()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &UTP_WeaponComponent::ReloadAnimEnded);
	}

	PlayerCamera = Character->GetFirstPersonCameraComponent();


	//if (Once)
	{

		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// Fire
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Canceled, this, &UTP_WeaponComponent::EndFire);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::EndFire);
				EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Reload);
			}
		}
		Once = false;
	}
}

void UTP_WeaponComponent::SetCanFire(bool Flag)
{
	if (!Flag)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}
	else
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(FireMappingContext, 2);
			}
		}
	}
		
	CanFire = Flag;
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

	//Bullet �ݵ� timeline ����
	if (RecoilStrengthCurve)
	{
		FOnTimelineVector CurveCallbak;
		CurveCallbak.BindUFunction(this, FName("OnBulletRecoilProgress"));

		RecoilOffset.AddInterpVector(RecoilStrengthCurve, CurveCallbak);
		RecoilOffset.SetLooping(true);

		RecoilOffset.Play();
	}

	auto MyGameInstance = Cast<UDefaultGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetBulletData(WeaponName);
		if (StatData)
		{
			DamageTable.Add(TEXT("Head"), StatData->HeadDamage);
			DamageTable.Add(TEXT("Body"), StatData->BodyDamage);
			DamageTable.Add(TEXT("LArm"), StatData->BodyDamage);
			DamageTable.Add(TEXT("RArm"), StatData->BodyDamage);
			DamageTable.Add(TEXT("RLeg"), StatData->LegDamage);
			DamageTable.Add(TEXT("LLeg"), StatData->LegDamage);
		}
	}
}

void UTP_WeaponComponent::ApplyCameraRecoil()
{
	//Camera �ݵ� ����
	if (Character)
	{
		FRotator NewCameraRotation = OriginalCameraRotation + RecoilRotation;
		GetWorld()->GetFirstPlayerController()->SetControlRotation(NewCameraRotation);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(NewCameraRotation.ToString()));
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(rot.ToString()));
	//GetWorld()->GetFirstPlayerController()->PlayerCameraManager->
}

//�߻� ����
void UTP_WeaponComponent::EndFire()
{
	//���� ���� �ʱ�ȭ
	//RecoilRotation = FRotator::ZeroRotator;

	bFiring = false;
	RemainNum = 0;
	RightTurn = false;
	RecoilOffset.SetPlaybackPosition(0, false);
	BulletOffset = FVector::ZeroVector;
}

void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//�߻�(����)�� �̶�� timeline ����
	if (bFiring)
	{
		RecoilOffset.TickTimeline(DeltaTime);
	}

	//Camera ���� ����
	if (Character)
	{
		if (auto Controller = Character->GetController())
		{
			//RecoilRotation�� ���ܵ� origin ����
			OriginalCameraRotation = Controller->GetControlRotation() - RecoilRotation;

			if (!RecoilRotation.IsNearlyZero(0.1))
			{
				FRotator SmoothedRotation = FMath::RInterpTo(RecoilRotation, FRotator::ZeroRotator, GetWorld()->GetDeltaSeconds(), RecoilRecoveryTime);
				RecoilRotation = SmoothedRotation;
				Controller->SetControlRotation(OriginalCameraRotation + SmoothedRotation);
			}
			else
			{
				RecoilRotation = FRotator::ZeroRotator;
			}
		}
	}
}

void UTP_WeaponComponent::OnBulletRecoilProgress(FVector BulletRecoil)
{
	//Bullet �ݵ��� ����
	//Z: ����, Y: �˿�
	auto tempZ = FMath::RandRange(-.01f, .03f);
	auto tempY = FMath::RandRange(-.1f, .3f);
	BulletOffset = { 0, tempY, tempZ };

	BulletOffset *= BulletRecoil;
}

void UTP_WeaponComponent::Reload()
{
	CanFire = false;
	//AnimMontage Play
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ReloadAnimation, 1.f);
	}
}

//������ Anim �Ϸ�� �����Ϸ� & ���� ź ���
void UTP_WeaponComponent::ReloadAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	auto Name = Montage->GetName();
	if (!bInterrupted && Name.Equals(ReloadName))
	{
		auto Owner = Cast<AGun>(GetOwner());
		if (Owner)
		{
			Owner->Reload();
		}
	}

	if (!bInterrupted && Name.Equals(TEXT("Rifle_Shoot")))
	{
		EndFire();
	}
}
