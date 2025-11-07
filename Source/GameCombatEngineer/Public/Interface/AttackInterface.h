// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackInterface.generated.h"

DECLARE_DYNAMIC_DELEGATE(FExitCombatDelegate);
// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMECOMBATENGINEER_API IAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FExitCombatDelegate I_OnExitCombat;
public:
	virtual void I_PlayAttackMontage(UAnimMontage* AttackMontage) = 0;
	virtual void I_EndAttackMontage() = 0;
	virtual FVector I_GetSocketLocation(const FName& SocketName) const = 0;

	virtual void I_ANS_TraceHit() = 0;
	virtual void I_ANS_BeginTraceHit() = 0;
	virtual void I_ANS_Combo() = 0;

	virtual void I_EnterCombat(float Health_Target,float MaxHealth_Target, float Stamina_Target, float MaxStamina_Target);
	virtual void I_ExitCombat();
	virtual void I_HitTarget(float Health_Target, float MaxHealth_Target);

	virtual void I_EndHitReact() = 0;
	virtual void I_HandleTargetDestroy();

	virtual void I_HandleAttackSuccess() = 0;
	virtual void I_HandleTargetAttackSuccess(float Stamina_Target, float MaxStamina_Target);
	virtual bool I_IsReadyAttack() const = 0;
	virtual bool I_CheckEnoughStamina(float Cost) const = 0;

	virtual void I_StaminaUpdate();
	virtual void I_HandleStaminaUpdateTarget(float Stamina_Target, float MaxStamina_Target);
	virtual bool I_IsAttacking() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual void I_RequestAttack() = 0;
};
