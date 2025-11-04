// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UPlayerWidget;

UCLASS()
class GAMECOMBATENGINEER_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void I_EnterCombat(float Health_Target, float MaxHealth_Target) override;
	virtual void I_ExitCombat() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY();
	UPlayerWidget* bPlayerWidget;
};
