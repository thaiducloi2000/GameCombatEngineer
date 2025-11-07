// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "Interface/AttackInterface.h"
#include "DataAsset/CharacterData.h"
#include "Controller/EnemyAIController.h"
#include "Component/HealthComponent.h"
#include "Component/StaminaComponent.h"

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
	if (CharacterData)
		ChangeWalkSpeed(CharacterData->CombatSpeed);

	Attacker = TScriptInterface<IAttackInterface>(PlayerActor);

	if (Attacker == nullptr) return;

	if (Attacker->I_OnExitCombat.IsBound() == false)
		Attacker->I_OnExitCombat.BindDynamic(this, &AEnemyCharacter::HandlePlayerExitCombat);

	if (HealthComponent && StaminaComponent) {
		Attacker->I_EnterCombat(HealthComponent->Health, HealthComponent->MaxHealth, StaminaComponent->Stamina, StaminaComponent->MaxStamina);
	}
}

void AEnemyCharacter::I_HandleLostPlayer(AActor* PlayerActor)
{
	Attacker = TScriptInterface<IAttackInterface>(PlayerActor);
	if (Attacker) {
		Attacker->I_ExitCombat();
	}
}

void AEnemyCharacter::Destroyed()
{
	if (Attacker) {
		Attacker->I_HandleTargetDestroy();
	}

	Super::Destroyed();
}

void AEnemyCharacter::I_HandleAttackSuccess()
{
	Super::I_HandleAttackSuccess();
	if (Attacker && StaminaComponent)
		Attacker->I_HandleTargetAttackSuccess(StaminaComponent->Stamina, StaminaComponent->MaxStamina);
}

void AEnemyCharacter::I_StaminaUpdate()
{
	if (Attacker && StaminaComponent) {
		Attacker->I_HandleStaminaUpdateTarget(StaminaComponent->Stamina, StaminaComponent->MaxStamina);
	}
}

void AEnemyCharacter::HandlePlayerExitCombat()
{
	auto EnemyAIController = Cast<AEnemyAIController>(GetController());

	if (EnemyAIController)
		EnemyAIController->BackToPatrol();
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

void AEnemyCharacter::HandleDead()
{
	Super::HandleDead();
	DetachFromControllerPendingDestroy();
}
