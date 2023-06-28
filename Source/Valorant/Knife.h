// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.h"
#include "Knife.generated.h"

UCLASS()
class VALORANT_API AKnife : public AWeapon
{
	GENERATED_BODY()

	bool CanAttack = true;
public:	
	// Sets default values for this actor's properties
	AKnife();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCanAttack(bool flag) { CanAttack = flag; };
	bool GetCanAttack() { return CanAttack; };
};
