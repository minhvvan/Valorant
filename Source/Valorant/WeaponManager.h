// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VALORANT_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TMap<FString, class AWeapon*> Weapons;

	class AValorantCharacter* Character;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AWeapon> KnifeClass;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//�߰�
	bool AddWeapon(AWeapon* Weapon);
	//����
	void RemoveWeapon(AWeapon* Weapon);
	//����
	void ChangeWeapon(AWeapon* Weapon);


	//���� ���� ����
	void SwapWeapon(FString Tag);
};
