// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Flash.h"
#include "Components/WidgetComponent.h"
#include "Animation/WidgetAnimation.h"

void UWidget_Flash::NativeOnInitialized()
{
	maxTime = 2.5;

	EndDelegate.BindDynamic(this, &UWidget_Flash::AnimationFinished);
	BindToAnimationFinished(Flash, EndDelegate);
}

void UWidget_Flash::PlayAnim(float time)
{
	auto startTime = maxTime - time;

	if (Flash && !IsPlayingAnimation())
	{
		PlayAnimation(Flash, startTime);
	}
}

void UWidget_Flash::AnimationFinished()
{
	//UE_LOG(LogTemp, Warning, TEXT("finish"));
	RemoveFromParent();
}


