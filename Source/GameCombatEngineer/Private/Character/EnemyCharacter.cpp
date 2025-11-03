// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

FVector AEnemyCharacter::I_GetPatrolLocation()
{
	if (PatrolPoints.IsEmpty()) return GetActorLocation();
	if(PatrolPoints[PatrolIndex] == nullptr) return GetActorLocation();

	auto result = PatrolPoints[PatrolIndex]->GetActorLocation();
	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	return result;
}
