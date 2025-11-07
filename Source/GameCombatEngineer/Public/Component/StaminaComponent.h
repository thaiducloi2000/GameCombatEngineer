// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

class UCharacterData;
class IAttackInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECOMBATENGINEER_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStaminaComponent();
	void SetupComponent(UCharacterData* CharacterData);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);
	void UpdateStamina(float Cost);
protected:
	virtual void BeginPlay() override;
private :
	UPROPERTY()
	TScriptInterface<IAttackInterface> AttackInterface;
public:	
	float Stamina = 0.0f;
	float MaxStamina = 0.0f;
	float RegenSpeed = 0.0f;
};
