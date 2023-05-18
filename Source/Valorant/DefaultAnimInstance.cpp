// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAnimInstance.h"

UDefaultAnimInstance::UDefaultAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RM(TEXT("/Script/Engine.AnimMontage'/Game/FirstPersonArms/Animations/FP_Reload.FP_Reload'"));

	if (RM.Succeeded())
	{
		ReloadMontage = RM.Object;
	}
}

void UDefaultAnimInstance::PlayReloadMontage()
{
	if (ReloadMontage)
	{
	UE_LOG(LogTemp, Warning, TEXT("PlayMontage"));
		if (!Montage_IsPlaying(ReloadMontage))
		{
			//Montage_Play(ReloadMontage, 1.f);
		}
	}
}
