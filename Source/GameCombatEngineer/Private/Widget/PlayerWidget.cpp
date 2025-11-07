// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerWidget::UpdateHealthBar_Player(float Health, float MaxHealth)
{
	if (HealthBar_Player)
		HealthBar_Player->SetPercent(Health / MaxHealth);
}

void UPlayerWidget::UpdateHealthBar_Enemy(float Health, float MaxHealth)
{
	if (HealthBar_Enemy)
		HealthBar_Enemy->SetPercent(Health / MaxHealth);

	FString healthString = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));

	if (HealthText_Enemy)
		HealthText_Enemy->SetText(FText::FromString(healthString));
}

void UPlayerWidget::UpdateStaminaBar_Player(float Stamina, float MaxStamina)
{
	if (StaminaBar_Player)
		StaminaBar_Player->SetPercent(Stamina / MaxStamina);
}

void UPlayerWidget::UpdateStaminaBar_Enemy(float Stamina, float MaxStamina)
{
	if (StaminaBar_Enemy)
		StaminaBar_Enemy->SetPercent(Stamina / MaxStamina);
}

void UPlayerWidget::ShowEnemyStat()
{
	if (HealthBar_Enemy)
		HealthBar_Enemy->SetVisibility(ESlateVisibility::Visible);
	if (HealthText_Enemy)
		HealthText_Enemy->SetVisibility(ESlateVisibility::Visible);
	if(StaminaBar_Enemy)
		StaminaBar_Enemy->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerWidget::HideEnemyStat()
{
	if (HealthBar_Enemy)
		HealthBar_Enemy->SetVisibility(ESlateVisibility::Hidden);
	if (HealthText_Enemy)
		HealthText_Enemy->SetVisibility(ESlateVisibility::Hidden);
	if (StaminaBar_Enemy)
		StaminaBar_Enemy->SetVisibility(ESlateVisibility::Hidden);
}
