// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillPawn.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ASkillPawn::ASkillPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASkillPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASkillPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASkillPawn::Fire(FVector Direction)
{

}

