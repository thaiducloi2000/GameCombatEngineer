// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Widget/PlayerWidget.h"
#include "Component/HealthComponent.h"

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	auto PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

	if (PlayerWidget != nullptr && HealthComponent != nullptr) {
		PlayerWidget->AddToViewport();
		PlayerWidget->UpdateHealthBar_Player(HealthComponent->Health / HealthComponent->MaxHealth);
	}
}
