// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Flash.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UWidget_Flash : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized();

public:
	void PlayAnim(float time);

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Flash;

private:
	float maxTime = 2.5;

	FWidgetAnimationDynamicEvent EndDelegate;

	UFUNCTION()
	void AnimationFinished();
};
