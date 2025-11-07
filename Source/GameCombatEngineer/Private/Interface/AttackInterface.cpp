// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/AttackInterface.h"

// Add default functionality here for any IAttackInterface functions that are not pure virtual.

void IAttackInterface::I_EnterCombat(float Health_Target, float MaxHealth_Target, float Stamina_Target, float MaxStamina_Target)
{

}

void IAttackInterface::I_ExitCombat()
{
}

void IAttackInterface::I_HitTarget(float Health_Target, float MaxHealth_Target)
{
}

void IAttackInterface::I_HandleTargetDestroy()
{
}

void IAttackInterface::I_HandleTargetAttackSuccess(float Stamina_Target, float MaxStamina_Target)
{
}

void IAttackInterface::I_StaminaUpdate()
{
}

void IAttackInterface::I_HandleStaminaUpdateTarget(float Stamina_Target, float MaxStamina_Target)
{
}
