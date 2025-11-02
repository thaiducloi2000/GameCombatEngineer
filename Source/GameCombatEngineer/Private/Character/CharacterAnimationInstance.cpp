// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterAnimationInstance.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enum/CombatState.h"

void UCharacterAnimationInstance::NativeInitializeAnimation()
{
	Character = Cast<ABaseCharacter>(TryGetPawnOwner());

	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UCharacterAnimationInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (MovementComponent)
	{
		Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
	}

	if (Character == nullptr) return;
	Character->GetCombatState();
	bShouldBlendLowerUpper = Speed > 0.0f && Character->GetCombatState() != ECombatState::Beaten;
}
