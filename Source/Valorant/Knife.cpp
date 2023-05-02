// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AKnife::AKnife()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Knife/knife_Low.knife_Low'"));
	if (SM.Succeeded())
	{
		Mesh->SetStaticMesh(SM.Object);
	}
	SetRootComponent(Mesh);

	WeaponTag = FName(TEXT("Knife"));
}

// Called when the game starts or when spawned
void AKnife::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetCollisionProfileName(TEXT("IgnoreAll"));
}

// Called every frame
void AKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


