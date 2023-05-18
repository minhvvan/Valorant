// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DefaultAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UDefaultAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UDefaultAnimInstance();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	UAnimMontage* ReloadMontage;

	void PlayReloadMontage();
};
