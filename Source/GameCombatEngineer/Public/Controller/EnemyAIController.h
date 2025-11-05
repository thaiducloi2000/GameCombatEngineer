// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class IEnemyInterface;
class UAISenseConfig_Sight;

UCLASS()
class GAMECOMBATENGINEER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

	UFUNCTION(BlueprintCallable)
	void UpdatePatrolLocation();

	UFUNCTION(BlueprintCallable)
	void CheckDistance(AActor* AIActor, AActor* Player, float AttackRange);

private:
	UFUNCTION()
	void HandleActorPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);
	void SeePlayer(AActor* Actor);
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	TScriptInterface<IEnemyInterface> EnemyInterface;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
	FName Key_PatrolLocation;

	UPROPERTY(EditDefaultsOnly)
	FName Key_ShouldAttack;

	UPROPERTY(EditDefaultsOnly)
	FName Key_IsCombat;

	UPROPERTY(EditDefaultsOnly)
	FName Key_PlayerActor;


	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere)
	UAISenseConfig_Sight* AISightConfig;
};
