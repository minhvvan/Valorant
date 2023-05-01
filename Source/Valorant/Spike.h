// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spike.generated.h"

UCLASS()
class VALORANT_API ASpike : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpike();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTP_PickUpComponent* PickUpComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PickUp(class AValorantCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = Spike)
	bool GetCanInstall() { return CanInstall; };
	
	UFUNCTION(BlueprintCallable, Category = Spike)
	void SetCanInstall(bool flag) { CanInstall = flag; };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanInstall = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanPickUp = true;
};
