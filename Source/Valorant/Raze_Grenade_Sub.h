// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Raze_Grenade.h"
#include "Raze_Grenade_Sub.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ARaze_Grenade_Sub : public ARaze_Grenade
{
	GENERATED_BODY()
	
public:
	ARaze_Grenade_Sub();

	UFUNCTION()
	virtual void Explosion() override;
};
