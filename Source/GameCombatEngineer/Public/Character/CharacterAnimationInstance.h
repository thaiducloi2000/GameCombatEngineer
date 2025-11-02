// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimationInstance.generated.h"

class ABaseCharacter;
class UCharacterMovementComponent;

UCLASS()
class GAMECOMBATENGINEER_API UCharacterAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(BlueprintReadOnly)
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	bool bShouldBlendLowerUpper = true;
private:

	UPROPERTY()
	ABaseCharacter* Character;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
};
