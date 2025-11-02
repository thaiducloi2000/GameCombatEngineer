// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Character.h"
#include "Interface/AttackInterface.h"
#include "DataAsset/CharacterData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Component/AttackComponent.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AttackInterface = TScriptInterface<IAttackInterface>(GetOwner());
}

void UAttackComponent::RequestAttack()
{
	const bool bCanAttack = bIsAttacking == false || bCanCombo == true;
	if (bCanAttack) Attack();
	else bSaveAttack = true;
}

void UAttackComponent::Attack()
{
	if (AttackInterface != nullptr && CharacterData != nullptr && GetAttackMontage() != nullptr)
	{
		AttackInterface->I_PlayAttackMontage(GetAttackMontage());

		bIsAttacking = true;
		bCanCombo = false;

		bAttackIndex = (bAttackIndex + 1) % CharacterData->AttackMontages.Num();
	}
}

void UAttackComponent::EndAttack()
{
	bCanCombo = false;
	bIsAttacking = false;
	bSaveAttack = false;
	bAttackIndex = 0;
}

void UAttackComponent::Combo()
{
	bCanCombo = true;
	if (bSaveAttack) 
	{
		RequestAttack();
		bSaveAttack = false;
	}
}

void UAttackComponent::TraceHit()
{
	if (AttackInterface == nullptr) return;
	if (CharacterData == nullptr) return;
	const FVector& StartLocation = AttackInterface->I_GetSocketLocation(CharacterData->TraceStart);
	const FVector& EndLocation = AttackInterface->I_GetSocketLocation(CharacterData->TraceEnd);
	TArray<FHitResult> HitResults;

	bool IsHitSomething = UKismetSystemLibrary::SphereTraceMultiForObjects(this,
		StartLocation, EndLocation,
		CharacterData->TraceRadius,
		CharacterData->TraceObjectType,
		false,
		CharacterData->ActorToIgnore,
		CharacterData-> bDrawDebugTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		HitResults, true,
		FLinearColor::Red,
		FLinearColor::Green);
	if (!IsHitSomething) return;

	for (const FHitResult& Result : HitResults)
	{
		AActor* Actor = Result.GetActor();
		if (HittedActor.Contains(Actor)) continue;
		HandleHitResult(Result);
		UKismetSystemLibrary::DrawDebugSphere(this, Result.ImpactPoint, 10.0f);

		HittedActor.Emplace(Actor);
	}
}

void UAttackComponent::HandleHitResult(const FHitResult& Result)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, Result.BoneName.ToString());
	if (HitSomethingDelegate.IsBound())
		HitSomethingDelegate.Execute(Result);
}

UAnimMontage* UAttackComponent::GetAttackMontage()
{
	if (CharacterData == nullptr) return nullptr;
	if (CharacterData->AttackMontages.IsEmpty()) return nullptr;
	return CharacterData->AttackMontages[bAttackIndex];
	
}

void UAttackComponent::SetupTraceHit()
{
	HittedActor.Empty();
}

