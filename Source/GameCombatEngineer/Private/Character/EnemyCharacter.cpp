// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "Interface/AttackInterface.h"
#include "Component/HealthComponent.h"

FVector AEnemyCharacter::I_GetPatrolLocation()
{
	if (PatrolPoints.IsEmpty()) return GetActorLocation();
	if(PatrolPoints[PatrolIndex] == nullptr) return GetActorLocation();

	auto result = PatrolPoints[PatrolIndex]->GetActorLocation();
	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	return result;
}

void AEnemyCharacter::I_HandleSeePlayer(AActor* PlayerActor)
{
	auto Attacker = TScriptInterface<IAttackInterface>(PlayerActor);
	if (Attacker && HealthComponent) {
		Attacker->I_EnterCombat(HealthComponent->Health, HealthComponent->MaxHealth);
	}
}

void AEnemyCharacter::I_HandleLostPlayer(AActor* PlayerActor)
{
	auto Attacker = TScriptInterface<IAttackInterface>(PlayerActor);
	if (Attacker) {
		Attacker->I_ExitCombat();
	}
}
