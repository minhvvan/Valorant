// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillPawn.h"
#include "Raze_Blast.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API ARaze_Blast : public ASkillPawn
{
	GENERATED_BODY()
	
public:
	ARaze_Blast();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	virtual void Fire(FVector Direction) override;

	virtual void Explosion();

	UPROPERTY(EditAnywhere, Category = "Setting")
	float PendingTime = 3.5f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float Range = 150.f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float Damage = 50.f;	
	
	UPROPERTY(EditAnywhere, Category = "Setting")
	float Power = 50.f;

	FTimerHandle ExplosionTimerHandle;

	class AValorantCharacter* Character;

	void SetCharacter(class AValorantCharacter* owner) { Character = owner; };

	void CheckHit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	UMaterial* Paint;

	TSet<int> VictimSet;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
protected:
	virtual void PostInitializeComponents();
};
