// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "ValorantCharacter.h"
#include "TP_InteractionComponent.generated.h"

// The character picking this up is the parameter sent with the notification
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, AValorantCharacter*, InteractCharacter);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UTP_InteractionComponent : public USphereComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTP_InteractionComponent();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteract OnInteract;	
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractEnd OnInteractEnd;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
