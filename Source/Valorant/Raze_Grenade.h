// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillPawn.h"
#include "Raze_Grenade.generated.h"

class AValorantCharacter;
/**
 * 
 */
UCLASS()
class VALORANT_API ARaze_Grenade : public ASkillPawn
{
	GENERATED_BODY()

public:
	ARaze_Grenade();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class USphereComponent* CollisionComp;
	
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	virtual void Fire(FVector Direction) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void Explosion();

	UPROPERTY(EditAnywhere, Category = "Setting")
	float PendingTime = 3.5f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float Range = 150.f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float Damage = 50.f;

	FTimerHandle ExplosionTimerHandle;

	class AValorantCharacter* Character;

	void SetCharacter(class AValorantCharacter* owner) { Character = owner; };

	void CheckHit();

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<class ASkillPawn> SubGrenadeClass;

	//class ARaze_Grenade_Sub* SubGrenade;

	TArray<int> dy;
	TArray<int> dx;
};
