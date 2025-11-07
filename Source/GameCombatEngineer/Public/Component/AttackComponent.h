// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttackInterface.h"
#include "Enum/AttackType.h"
#include "AttackComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FHitSomethingDelegate, const FHitResult&, HitResult);

class UAnimMontage;
class ACharacter;
class UCharacterData;
class IAttackInterface;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMECOMBATENGINEER_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttackComponent();
	void RequestAttack();
	bool CanAttack() const;
	void Attack();
	void EndAttack();
	void Combo();
	void TraceHit();
	void SetupTraceHit();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	void HandleHitResult(const FHitResult& Result);
	UAnimMontage* GetAttackMontage();
public:
	FHitSomethingDelegate HitSomethingDelegate;
	EAttackType RequestAttackType = EAttackType::Normal;
	bool bIsAttacking = false;
private:
	UPROPERTY()
	TScriptInterface<IAttackInterface> AttackInterface;

	UPROPERTY(EditDefaultsOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UCharacterData* CharacterData;


	TArray<AActor*> HittedActor;
	bool bCanCombo = false;
	bool bSaveAttack = false;
	int bAttackIndex = 0;
};
