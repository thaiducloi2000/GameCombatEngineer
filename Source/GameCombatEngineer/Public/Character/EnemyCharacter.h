// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interface/EnemyInterface.h"
#include "Interface/AttackInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAMECOMBATENGINEER_API AEnemyCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	virtual FVector I_GetPatrolLocation() override;

	virtual void I_HandleSeePlayer(AActor* PlayerActor) override;

	virtual void I_HandleLostPlayer(AActor* PlayerActor) override;
protected:
	virtual void BeginPlay() override;
	virtual void HandleTakePointDamage(AActor* DamagedActor,
		float Damage,
		AController* InstigatedBy,
		FVector HitLocation, UPrimitiveComponent*
		FHitComponent, FName BoneName,
		FVector ShotFromDirection,
		const class UDamageType* DamageType,
		AActor* DamageCauser) override;
private:
	UPROPERTY(EditInstanceOnly, Category = "Patrol");
	TArray<AActor*> PatrolPoints;

	int PatrolIndex;

	UPROPERTY()
	TScriptInterface<IAttackInterface> Attacker;
};
