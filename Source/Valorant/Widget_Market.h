// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Market.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UWidget_Market : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* ClassicBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ShortyBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* FrenzyBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* GhostBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* SheriffBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* StingerBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* SpectreBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* BuckyBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* JudgeBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* BulldogBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* GuardianBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* VandalBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* PhantomBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* MarshalBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* OperatorBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* AresBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* OdinBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* LightShieldBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* HeavyShieldBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* CSkillBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* QSkillBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ESkillBtn;

protected:
	virtual void NativeOnInitialized();

private:
	UFUNCTION(BlueprintCallable)
	void ClickClassicBtn();

	UFUNCTION(BlueprintCallable)
	void ClickShortyBtn();

	UFUNCTION(BlueprintCallable)
	void ClickFrenzyBtn();

	UFUNCTION(BlueprintCallable)
	void ClickGhostBtn();

	UFUNCTION(BlueprintCallable)
	void ClickSheriffBtn();

	UFUNCTION(BlueprintCallable)
	void ClickStingerBtn();

	UFUNCTION(BlueprintCallable)
	void ClickSpectreBtn();

	UFUNCTION(BlueprintCallable)
	void ClickBuckyBtn();

	UFUNCTION(BlueprintCallable)
	void ClickJudgeBtn();

	UFUNCTION(BlueprintCallable)
	void ClickBulldogBtn();

	UFUNCTION(BlueprintCallable)
	void ClickGuardianBtn();

	UFUNCTION(BlueprintCallable)
	void ClickVandalBtn();

	UFUNCTION(BlueprintCallable)
	void ClickPhantomBtn();

	UFUNCTION(BlueprintCallable)
	void ClickMarshalBtn();

	UFUNCTION(BlueprintCallable)
	void ClickOperatorBtn();

	UFUNCTION(BlueprintCallable)
	void ClickAresBtn();

	UFUNCTION(BlueprintCallable)
	void ClickOdinBtn();

	UFUNCTION(BlueprintCallable)
	void ClickLightShieldBtn();

	UFUNCTION(BlueprintCallable)
	void ClickHeavyShieldBtn();

	UFUNCTION(BlueprintCallable)
	void ClickCSkillBtn();

	UFUNCTION(BlueprintCallable)
	void ClickQSkillBtn();

	UFUNCTION(BlueprintCallable)
	void ClickESkillBtn();

	FLinearColor Clicked;

};
