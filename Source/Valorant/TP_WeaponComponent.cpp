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

UTP_WeaponComponent::UTP_WeaponComponent()
{
	MuzzleOffset = FVector(0.0f, 0.0f, 0.0f);

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Resources/IMG/ShotHole_Mat.ShotHole_Mat'"));

	if (Material.Object != NULL)
	{
		ShotHoleMat = (UMaterial*)Material.Object;
	}
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
			OriginalCameraRotation = PlayerCamera->GetRelativeRotation();
			bFiring = true;
		}

		//������ �ݵ� ����
		auto randomRecoil = FMath::RandRange(0.5, 0.8);
		//~���� �ݵ�
		if (TargetCameraRotation.Pitch < MaxCameraRecoil)
		{
			TargetCameraRotation.Pitch += (RecoilStrength * randomRecoil);
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
				TargetCameraRotation.Yaw += (RecoilStrength * randomRecoil);
				if (TargetCameraRotation.Yaw > MaxRightYaw)
				{
					TargetCameraRotation.Yaw = MaxRightYaw;
				}
			}
			//���� �ݵ�
			else
			{
				TargetCameraRotation.Yaw -= (RecoilStrength * randomRecoil);
				if (TargetCameraRotation.Yaw < MaxLeftYaw)
				{
					TargetCameraRotation.Yaw = MaxLeftYaw;
				}
			}
		}
		//�ݵ� ����
		ApplyCameraRecoil();
	}

	//~HIt Check & Bullet Recoil
	{
		//Hit ���� ����
		FVector CameraLoc = PlayerCamera->GetComponentLocation();
		//Add Muzzle Offset
		CameraLoc += MuzzleOffset;

		FVector CameraForward = PlayerCamera->GetForwardVector();
		FVector StartLoc = CameraLoc;
		FVector EndLoc = CameraLoc + ((CameraForward + BulletOffset) * Range);

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
		auto Weapons = Character->GetWeapons();
		for (auto& weapon : Weapons)
		{
			IgnoreActors.Add(weapon.Value);
		}

		FHitResult HitResult;

		//��Ʈ ����
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
			UGameplayStatics::ApplyDamage(victim, 10, Character->GetController(), Character, NULL);
		}
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

	//SetBulletWidget();

	PlayerCamera = Character->GetFirstPersonCameraComponent();

	//Attach
	//FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	//auto Owner = Cast<AWeapon>(GetOwner());
	//Character->WeaponManager->AddWeapon(Owner);

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

//void UTP_WeaponComponent::DetachWeapon()
//{
//	//�߻� �Ұ� & ������ ����
//	SetCanFire(false);
//	Character = nullptr;
//	Once = true;
//
//	//if (WidgetBullet)
//	//{
//	//	WidgetBullet->RemoveFromViewport();
//	//}
//}

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

//void UTP_WeaponComponent::SetBulletWidget()
//{
//	//ź Widget ����
//	if (WidgetBullet)
//	{
//		CurrentBullet = ReloadBullet;
//		WidgetBullet->AddToViewport();
//		WidgetBullet->SetCurrentBullet(CurrentBullet);
//		WidgetBullet->SetRemainBullet(RemainBullet);
//	}
//	else {
//		if (MainHUDWidgetClass)
//		{
//			WidgetBullet = Cast<UBulletWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));
//			if (IsValid(WidgetBullet))
//			{
//				// ������ ����Ʈ�� ���� �Լ�
//				CurrentBullet = ReloadBullet;
//				WidgetBullet->AddToViewport();
//				WidgetBullet->SetCurrentBullet(CurrentBullet);
//				WidgetBullet->SetRemainBullet(RemainBullet);
//			}
//		}
//	}
//}

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
}

void UTP_WeaponComponent::ApplyCameraRecoil()
{
	//Camera �ݵ� ����
	if (PlayerCamera)
	{
		FRotator NewCameraRotation = OriginalCameraRotation + TargetCameraRotation;
		PlayerCamera->SetRelativeRotation(NewCameraRotation);
	}
}

//�߻� ����
void UTP_WeaponComponent::EndFire()
{
	//���� ���� �ʱ�ȭ
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

	//�߻�(����)�� �̶�� timeline ����
	if (bFiring)
	{
		RecoilOffset.TickTimeline(DeltaTime);
	}

	//Camera ���� ����
	if (PlayerCamera)
	{
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
}
