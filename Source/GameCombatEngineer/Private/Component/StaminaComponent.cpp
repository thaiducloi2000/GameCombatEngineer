// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAsset/CharacterData.h"
#include "Component/StaminaComponent.h"
#include "Interface/AttackInterface.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(0.015f);
}

void UStaminaComponent::SetupComponent(UCharacterData* CharacterData)
{
	if (CharacterData) {
		Stamina = CharacterData->Stamina;
		MaxStamina = CharacterData->MaxStamina;
		RegenSpeed = CharacterData->RegenSpeed;
	}
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	AttackInterface = TScriptInterface<IAttackInterface>(GetOwner());
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RegenStamina(DeltaTime);
}

void UStaminaComponent::RegenStamina(float DeltaTime)
{
	if (Stamina >= MaxStamina) return;
	if (AttackInterface && AttackInterface->I_IsAttacking() == false) {
		Stamina = FMath::Min(Stamina + RegenSpeed * DeltaTime, MaxStamina);

		AttackInterface->I_StaminaUpdate();
	}

}

void UStaminaComponent::UpdateStamina(float Cost)
{
	Stamina = FMath::Max(Stamina - Cost, 0.0f);
}

