// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterAnimationInstance.h"
#include "GameFramework//Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimationInstance::NativeInitializeAnimation()
{
	Character = Cast<ACharacter>(TryGetPawnOwner());

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
}
