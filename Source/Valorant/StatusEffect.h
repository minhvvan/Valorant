// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class VALORANT_API StatusEffect
{
public:
	StatusEffect(float t, float r, AActor* owner);

	virtual void Fire() = 0;

protected:
	float time = 0;
	float range = 0;
	AActor* Owner;
};


class VALORANT_API SEFlash: public StatusEffect
{
public:
	SEFlash(float t, float r, AActor* owner);

	virtual void Fire();

private:
	const float FOV = 90.f;
};


class VALORANT_API SEConcussion: public StatusEffect
{
public:
	SEConcussion(float t, float r, AActor* owner);

	virtual void Fire();
};