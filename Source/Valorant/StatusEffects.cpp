// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffects.h"
#include "Kismet/GameplayStatics.h"
#include "ValorantCharacter.h"
#include "Widget_Flash.h"
#include "Engine/PostProcessVolume.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"

UStatusEffects::UStatusEffects()
{
}


/*----------------------------------------
SEFlash
-----------------------------------------*/


USEFlash::USEFlash()
{
	static ConstructorHelpers::FClassFinder<UWidget_Flash> UW(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widget/WBP_Flash.WBP_Flash_C'"));
	if (UW.Succeeded())
	{
		FlashClass = UW.Class;
	}
}

void USEFlash::Fire(const AValorantCharacter* Victim)
{
	UE_LOG(LogTemp, Warning, TEXT("fire"));

	if (Owner)
	{
		//sphere trace
		FVector Center = Owner->GetActorLocation();

		auto Foward = Victim->GetActorForwardVector();
		auto ToFlash = Center - Victim->GetActorLocation();
		ToFlash.Normalize();

		auto dotResult = Foward.Dot(ToFlash);
		float theta = FMath::RadiansToDegrees(FMath::Acos(dotResult));

		auto Cam = Victim->GetFirstPersonCameraComponent();
		if (Cam)
		{
			if (FlashWidget == nullptr)
			{
				if (FlashClass)
				{
					FlashWidget = Cast<UWidget_Flash>(CreateWidget(Owner->GetWorld(), FlashClass));
				}
			}

			FVector2D Loc;
			auto Controller = Victim->GetLocalViewingPlayerController();
			Controller->ProjectWorldLocationToScreen(Owner->GetActorLocation(), Loc);

			auto diffX = Loc.X - MidX;
			auto diffY = Loc.Y - MidY;

			FVector2D diff = { diffX, diffY };
			FlashWidget->SetPositionInViewport(diff);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *diff.ToString());

			FlashWidget->AddToPlayerScreen();

			//4단계로 분류
			if (theta <= FOV / 5.f)
			{
				// time * 1. 적용
				if (FlashWidget)
				{
					FlashWidget->PlayAnim(time);
				}
			}
			else if (theta <= FOV / 3.f)
			{
				// time * .7 적용
				if (FlashWidget)
				{
					FlashWidget->PlayAnim(time * .7f);
				}
			}
			else if (theta <= FOV / 2.f)
			{
				// time * .3 적용
				if (FlashWidget)
				{
					FlashWidget->PlayAnim(time * .3f);
				}
			}
			else
			{
				// time * .15 적용
				if (FlashWidget)
				{
					FlashWidget->PlayAnim(time * .15f);
				}
			}
		}
	}
}




/*----------------------------------------
SEConcussion
-----------------------------------------*/

USEConcussion::USEConcussion()
{
}

void USEConcussion::Fire(const AValorantCharacter* Victim)
{
	auto Cam = Victim->GetFirstPersonCameraComponent();
	if (Cam)
	{
		Cam->SetPostProcessBlendWeight(1.f);

		Victim->GetWorldTimerManager().SetTimer(
			TimerHandle, 
			FTimerDelegate::CreateLambda([Cam]() {
				Cam->SetPostProcessBlendWeight(0.f);
			}), time, false, -1.f);
	}
}
