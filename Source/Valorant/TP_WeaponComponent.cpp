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

//발사
void UTP_WeaponComponent::Fire()
{
	//Attach된 Character가 없으면 return
	if (Character == nullptr || Character->GetController() == nullptr || !CanFire)
	{
		return;
	}

	//~~Camera Recoil
	if (PlayerCamera)
	{
		//초탄
		if (!bFiring)
		{
			//조준점이 회복될 위치 저장
			OriginalCameraRotation = PlayerCamera->GetRelativeRotation();
			bFiring = true;
		}

		//무작위 반동 생성
		auto randomRecoil = FMath::RandRange(0.5, 0.8);
		//~위쪽 반동
		if (TargetCameraRotation.Pitch < MaxCameraRecoil)
		{
			TargetCameraRotation.Pitch += (RecoilStrength * randomRecoil);
		}
		else
		{
			//~좌우 반동
			//방향을 바꾸기까지 남은 탄 수
			if (RemainNum > 0)
			{
				RemainNum--;
			}
			//방향 전환
			else if (RemainNum == 0)
			{
				RightTurn = !RightTurn;
				RemainNum = FMath::RandRange(5, 13);
			}

			//우측 반동
			if (RightTurn)
			{
				TargetCameraRotation.Yaw += (RecoilStrength * randomRecoil);
				if (TargetCameraRotation.Yaw > MaxRightYaw)
				{
					TargetCameraRotation.Yaw = MaxRightYaw;
				}
			}
			//좌측 반동
			else
			{
				TargetCameraRotation.Yaw -= (RecoilStrength * randomRecoil);
				if (TargetCameraRotation.Yaw < MaxLeftYaw)
				{
					TargetCameraRotation.Yaw = MaxLeftYaw;
				}
			}
		}
		//반동 적용
		ApplyCameraRecoil();
	}

	//~HIt Check & Bullet Recoil
	{
		//Hit 판정 세팅
		FVector CameraLoc = PlayerCamera->GetComponentLocation();
		//Add Muzzle Offset
		CameraLoc += MuzzleOffset;

		FVector CameraForward = PlayerCamera->GetForwardVector();
		FVector StartLoc = CameraLoc;
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

		//히트 판정
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

			//피격된 Actor Damage적용
			auto victim = HitResult.GetActor();
			UGameplayStatics::ApplyDamage(victim, 10, Character->GetController(), Character, NULL);
		}
	}

	//소리 Play
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

	//현재 탄 수 감소
	auto Owner = Cast<AGun>(GetOwner());
	if (Owner)
	{
		Owner->DecreaseCurrentBullet();
	}
}

//Actor에게 무기 소유권 
void UTP_WeaponComponent::AttachWeapon(AValorantCharacter* TargetCharacter, FString Tag)
{

	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	//장전 Anim Callback 설정
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
//	//발사 불가 & 소유권 해제
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
//	//탄 Widget 설정
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
//				// 위젯을 뷰포트에 띄우는 함수
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

	//Bullet 반동 timeline 설정
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
	//Camera 반동 적용
	if (PlayerCamera)
	{
		FRotator NewCameraRotation = OriginalCameraRotation + TargetCameraRotation;
		PlayerCamera->SetRelativeRotation(NewCameraRotation);
	}
}

//발사 종료
void UTP_WeaponComponent::EndFire()
{
	//관련 변수 초기화
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

	//발사(연사)중 이라면 timeline 진행
	if (bFiring)
	{
		RecoilOffset.TickTimeline(DeltaTime);
	}

	//Camera 복구 진행
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
	//Bullet 반동값 생성
	//Z: 상하, Y: 죄우
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

//재장전 Anim 완료시 장전완료 & 남은 탄 계산
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
