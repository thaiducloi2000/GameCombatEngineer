// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAsset/CharacterData.h"
#include "Component/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::SetupComponent(UCharacterData* CharacterData)
{
	if (CharacterData) {
		Health = CharacterData->Health;
		MaxHealth = CharacterData->MaxHealth;
	}
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

