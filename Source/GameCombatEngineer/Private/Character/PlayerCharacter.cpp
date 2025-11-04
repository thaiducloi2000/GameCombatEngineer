// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Widget/PlayerWidget.h"
#include "Component/HealthComponent.h"

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	bPlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

	if (bPlayerWidget != nullptr && HealthComponent != nullptr) {
		bPlayerWidget->AddToViewport();
		bPlayerWidget->UpdateHealthBar_Player(HealthComponent->Health, HealthComponent->MaxHealth);
		bPlayerWidget->HideEnemyStat();
	}
}

void APlayerCharacter::I_EnterCombat(float Health_Target, float MaxHealth_Target)
{
	if (bPlayerWidget != nullptr && HealthComponent != nullptr) {
		bPlayerWidget->ShowEnemyStat();
		bPlayerWidget->UpdateHealthBar_Enemy(Health_Target, MaxHealth_Target);
	}
}

void APlayerCharacter::I_ExitCombat()
{
	if (bPlayerWidget != nullptr && HealthComponent != nullptr) {
		bPlayerWidget->HideEnemyStat();
	}
}
