// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_InteractionComponent.h"

// Sets default values for this component's properties
UTP_InteractionComponent::UTP_InteractionComponent()
{
	SphereRadius = 30.f;
}


// Called when the game starts
void UTP_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UTP_InteractionComponent::OnSphereBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UTP_InteractionComponent::OnSphereEndOverlap);
}

void UTP_InteractionComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AValorantCharacter* Character = Cast<AValorantCharacter>(OtherActor);
	if (Character != nullptr)
	{
		OnInteract.Broadcast(Character);
	}
}

void UTP_InteractionComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnInteractEnd.Broadcast();
}
