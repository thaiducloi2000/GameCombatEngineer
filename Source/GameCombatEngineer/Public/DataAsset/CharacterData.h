// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterData.generated.h"

/**
 *
 */
UCLASS()
class GAMECOMBATENGINEER_API UCharacterData : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackMontages;
#pragma region Animation Montage

	UPROPERTY(EditDefaultsOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage_Front;

	UPROPERTY(EditDefaultsOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage_Back;

	UPROPERTY(EditDefaultsOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage_Right;

	UPROPERTY(EditDefaultsOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage_Left;
#pragma endregion

#pragma region Effect

	UPROPERTY(EditDefaultsOnly, Category = Effect, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitImpactEffect;
#pragma endregion


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float JumpZVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float AirControl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float MaxWalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float PatrollSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float CombatSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float MinAnalogWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float BrakingDecelerationWalking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float BrakingDecelerationFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float TargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	FVector LocalOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Tray Hit", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectType;

	UPROPERTY(EditDefaultsOnly, Category = "Tray Hit", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ActorToIgnore;

	UPROPERTY(EditDefaultsOnly, Category = "Tray Hit", meta = (AllowPrivateAccess = "true"))
	float TraceRadius = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Tray Hit", meta = (AllowPrivateAccess = "true"))
	FName TraceStart;

	UPROPERTY(EditDefaultsOnly, Category = "Tray Hit", meta = (AllowPrivateAccess = "true"))
	FName TraceEnd;

	UPROPERTY(EditDefaultsOnly, Category = "Tray Hit", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebugTrace = true;

	//Stat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.0f;
};
