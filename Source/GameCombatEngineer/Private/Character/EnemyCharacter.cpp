// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "Interface/AttackInterface.h"
#include "DataAsset/CharacterData.h"
#include "Component/HealthComponent.h"

FVector AEnemyCharacter::I_GetPatrolLocation()
{
	if (PatrolPoints.IsEmpty()) return GetActorLocation();
	if (PatrolPoints[PatrolIndex] == nullptr) return GetActorLocation();

	auto result = PatrolPoints[PatrolIndex]->GetActorLocation();
	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	return result;
}

void AEnemyCharacter::I_HandleSeePlayer(AActor* PlayerActor)
{
	Attacker = TScriptInterface<IAttackInterface>(PlayerActor);
	if (Attacker && HealthComponent) {
		Attacker->I_EnterCombat(HealthComponent->Health, HealthComponent->MaxHealth);
	}

	if (CharacterData)
		ChangeWalkSpeed(CharacterData->CombatSpeed);
}

void AEnemyCharacter::I_HandleLostPlayer(AActor* PlayerActor)
{
	Attacker = TScriptInterface<IAttackInterface>(PlayerActor);
	if (Attacker) {
		Attacker->I_ExitCombat();
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterData)
		ChangeWalkSpeed(CharacterData->PatrollSpeed);
}

void AEnemyCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	Super::HandleTakePointDamage(DamagedActor,
		Damage,
		InstigatedBy,
		HitLocation,
		FHitComponent,
		BoneName,
		ShotFromDirection,
		DamageType,
		DamageCauser);
	if (Attacker && HealthComponent)
		Attacker->I_HitTarget(HealthComponent->Health, HealthComponent->MaxHealth);
}
