// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BulletWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UBulletWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentBullet;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RemainBullet;

	void SetCurrentBullet(float bullet);
	void SetRemainBullet(float bullet);
};
