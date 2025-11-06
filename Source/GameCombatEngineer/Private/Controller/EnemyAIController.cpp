// Fill out your copyright notice in the Description page of Project Settings.

#include "Interface/EnemyInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Enum/AIState.h"

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

void AEnemyAIController::CheckDistance(AActor* AIActor, AActor* Player, float AttackRange)
{
	if (AIActor == nullptr) return;

	const float distance = AIActor->GetDistanceTo(Player);

	if (distance <= AttackRange) {
		if (Blackboard)
			Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Attack);
	}
	else {
		if (Blackboard)
			Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Combat);
	}
}

void AEnemyAIController::BackToPatrol()
{
	if (Blackboard)
		Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Patrol);

	GetWorldTimerManager().SetTimer(ExitCombatTimer,this,&AEnemyAIController::ExitCombatTimerFinished,ExitCombatSecond);
}

void AEnemyAIController::ExitCombatTimerFinished()
{
	if (AIPerceptionComponent && AIPerceptionComponent->OnTargetPerceptionUpdated.IsBound() == false)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::HandleActorPerceptionUpdate);
}

void AEnemyAIController::HandleActorPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed()) {
		SeePlayer(Actor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Lost Player"));
		EnemyInterface->I_HandleLostPlayer(Actor);
	}
}

void AEnemyAIController::SeePlayer(AActor* Actor)
{
	if (EnemyInterface)
		EnemyInterface->I_HandleSeePlayer(Actor);

	if (Blackboard)
	{
		Blackboard->SetValueAsEnum(Key_AIState, (uint8)EAIState::Combat);
		Blackboard->SetValueAsObject(Key_PlayerActor, Actor);
	}

	if (AIPerceptionComponent && AIPerceptionComponent->OnTargetPerceptionUpdated.IsBound()) {
		AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AEnemyAIController::HandleActorPerceptionUpdate);
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyInterface = TScriptInterface<IEnemyInterface>(InPawn);

	if (BehaviorTree != nullptr)
		RunBehaviorTree(BehaviorTree);

	if (AIPerceptionComponent && AIPerceptionComponent->OnTargetPerceptionUpdated.IsBound() == false)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::HandleActorPerceptionUpdate);
}
