// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Market.h"
#include "Components/Button.h"

void UWidget_Market::NativeOnInitialized()
{
	ClassicBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickClassicBtn);
	ShortyBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickShortyBtn);
	FrenzyBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickFrenzyBtn);
	GhostBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickGhostBtn);
	SheriffBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickSheriffBtn);
	StingerBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickStingerBtn);
	SpectreBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickSpectreBtn);
	BuckyBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickBuckyBtn);
	JudgeBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickJudgeBtn);
	BulldogBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickBulldogBtn);
	GuardianBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickGuardianBtn);
	VandalBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickVandalBtn);
	PhantomBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickPhantomBtn);
	MarshalBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickMarshalBtn);
	OperatorBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickOperatorBtn);
	AresBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickAresBtn);
	OdinBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickOdinBtn);
	LightShieldBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickLightShieldBtn);
	HeavyShieldBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickHeavyShieldBtn);	
	CSkillBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickCSkillBtn);
	QSkillBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickQSkillBtn);
	ESkillBtn->OnClicked.AddDynamic(this, &UWidget_Market::ClickESkillBtn);

	Clicked = FLinearColor(0.f, 1.f, 1.f, .7f);
}

void UWidget_Market::ClickClassicBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickClassicBtn"));
	ClassicBtn->SetBackgroundColor(Clicked);
}

void UWidget_Market::ClickShortyBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickShortyBtn"));
}

void UWidget_Market::ClickFrenzyBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickFrenzyBtn"));
}

void UWidget_Market::ClickGhostBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickGhostBtn"));
}

void UWidget_Market::ClickSheriffBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickSheriffBtn"));
}

void UWidget_Market::ClickStingerBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickStingerBtn"));
}

void UWidget_Market::ClickSpectreBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickSpectreBtn"));
}

void UWidget_Market::ClickBuckyBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickBuckyBtn"));
}

void UWidget_Market::ClickJudgeBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickJudgeBtn"));
}

void UWidget_Market::ClickBulldogBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickBulldogBtn"));
}

void UWidget_Market::ClickGuardianBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickGuardianBtn"));
}

void UWidget_Market::ClickVandalBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickVandalBtn"));
}

void UWidget_Market::ClickPhantomBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickPhantomBtn"));
}

void UWidget_Market::ClickMarshalBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickMarshalBtn"));
}

void UWidget_Market::ClickOperatorBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickOperatorBtn"));
}

void UWidget_Market::ClickAresBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickAresBtn"));
}

void UWidget_Market::ClickOdinBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickOdinBtn"));
}

void UWidget_Market::ClickLightShieldBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("Light"));
}

void UWidget_Market::ClickHeavyShieldBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("Heavy"));
}

void UWidget_Market::ClickCSkillBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickCSkillBtn"));
}

void UWidget_Market::ClickQSkillBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickQSkillBtn"));
}

void UWidget_Market::ClickESkillBtn()
{
	UE_LOG(LogTemp, Warning, TEXT("ClickESkillBtn"));
}
