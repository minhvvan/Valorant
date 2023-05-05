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

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	class UTP_InteractionComponent* InteractComp;

	UPROPERTY(EditAnywhere, BlueprintreadOnly)
	class UWidgetComponent* InteractUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanInteraction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PickUp(class AValorantCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = Spike)
	bool GetCanInstall() { return CanInstall; };
	
	UFUNCTION(BlueprintCallable, Category = Spike)
	void SetCanInstall(bool flag) { CanInstall = flag; };

	UFUNCTION(BlueprintCallable, Category = Spike)
	bool GetCanInteraction() { return bCanInteraction; };

	UFUNCTION(BlueprintCallable, Category = Spike)
	void SetCanInteraction(bool flag) { bCanInteraction = flag; };

	void Interact(AValorantCharacter* Character);
	void EndInteract();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanInstall = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanPickUp = true;

private:
	class AValorantCharacter* OverlappedCharacter;
};
