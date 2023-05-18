// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletWidget.h"
#include "Components/TextBlock.h"

void UBulletWidget::SetCurrentBullet(float bullet)
{
	auto text = FText::AsNumber(bullet);
	CurrentBullet->SetText(text);
}

void UBulletWidget::SetRemainBullet(float bullet)
{
	auto text = FText::AsNumber(bullet);
	RemainBullet->SetText(text);
}
