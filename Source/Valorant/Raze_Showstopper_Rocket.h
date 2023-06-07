// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillPawn.h"
#include "Raze_Showstopper_Rocket.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ARaze_Showstopper_Rocket : public ASkillPawn
{
	GENERATED_BODY()
	
public:
	ARaze_Showstopper_Rocket();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	virtual void Fire(FVector Direction) override;

	virtual void Explosion();
	void PaintDecal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	UMaterial* Paint;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float PendingTime = 3.5f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float Range = 150.f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float Damage = 150.f;

	FTimerHandle ExplosionTimerHandle;

	class AValorantCharacter* Character;

	void SetCharacter(class AValorantCharacter* owner) { Character = owner; };

	void CheckHit();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	TSet<int> VictimSet;

protected:
	virtual void PostInitializeComponents();

};
