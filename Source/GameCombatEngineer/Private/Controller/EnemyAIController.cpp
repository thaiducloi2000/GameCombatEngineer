// Fill out your copyright notice in the Description page of Project Settings.

#include "Interface/EnemyInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/EnemyAIController.h"

void AEnemyAIController::UpdatePatrolLocation()
{
	if (EnemyInterface && Blackboard)
		Blackboard->SetValueAsVector(Key_PatrolLocation, EnemyInterface->I_GetPatrolLocation());
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyInterface = TScriptInterface<IEnemyInterface>(InPawn);

	if (BehaviorTree != nullptr)
		RunBehaviorTree(BehaviorTree);
}
