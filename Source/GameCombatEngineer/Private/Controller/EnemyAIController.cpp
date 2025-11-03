// Fill out your copyright notice in the Description page of Project Settings.

#include "Interface/EnemyInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	AISightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));

	if (AISightConfig) {
		AISightConfig->SightRadius = 2500.0f;
		AISightConfig->LoseSightRadius = 2500.0f;
		AISightConfig->PeripheralVisionAngleDegrees = 55.0f;
		AISightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		if (AIPerceptionComponent) {
			AIPerceptionComponent->ConfigureSense(*AISightConfig);
		}
	}
}

void AEnemyAIController::UpdatePatrolLocation()
{
	if (EnemyInterface && Blackboard)
		Blackboard->SetValueAsVector(Key_PatrolLocation, EnemyInterface->I_GetPatrolLocation());
}

void AEnemyAIController::HandleActorPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed()) {

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Find Player"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Lost Player"));
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyInterface = TScriptInterface<IEnemyInterface>(InPawn);

	if (BehaviorTree != nullptr)
		RunBehaviorTree(BehaviorTree);

	if (AIPerceptionComponent)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::HandleActorPerceptionUpdate);
}
