// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UPlayerWidget;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GAMECOMBATENGINEER_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void I_EnterCombat(float Health_Target, float MaxHealth_Target, float Stamina_Target, float MaxStamina_Target) override;
	virtual void I_ExitCombat() override;

	virtual void I_HitTarget(float Health_Target, float MaxHealth_Target) override;
	virtual void I_HandleTargetDestroy() override;
	virtual void I_HandleAttackSuccess() override;
	virtual void I_HandleTargetAttackSuccess(float Stamina_Target, float MaxStamina_Target) override;
	virtual void I_StaminaUpdate() override;
	virtual void I_HandleStaminaUpdateTarget(float Stamina_Target, float MaxStamina_Target) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void HandleTakePointDamage(AActor* DamagedActor,
		float Damage,
		AController* InstigatedBy,
		FVector HitLocation, UPrimitiveComponent*
		FHitComponent, FName BoneName,
		FVector ShotFromDirection,
		const class UDamageType* DamageType,
		AActor* DamageCauser) override;
	virtual void HandleDead() override;
	virtual void HandleBeaten(const FVector& HitLocation, const FVector& ShotFromDirection) override;
private:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/* Called for attack input*/
	void Attack(const FInputActionValue& Value);

	void StrongAttack(const FInputActionValue& Value);
	/* Called for run input*/
	void ExitCombat(const FInputActionValue& Value);

	/* Called for stop run input*/
	//void StopRun(const FInputActionValue& Value);
	void SetupCharacterData();

	virtual void PostInitializeComponents() override;

	virtual void NotifyControllerChanged() override;
private:

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY();
	UPlayerWidget* bPlayerWidget;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FollowCamera;
};
