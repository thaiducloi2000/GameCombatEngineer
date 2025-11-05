#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/AttackInterface.h"
#include "Enum/CombatState.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UInputData;
class UCharacterData;
class UAttackComponent;
class UHealthComponent;

UCLASS()
class GAMECOMBATENGINEER_API ABaseCharacter : public ACharacter, public IAttackInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputData* InputData;

	UPROPERTY(EditDefaultsOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UCharacterData* CharacterData;

	UPROPERTY(EditDefaultsOnly, Category = AttackComponent, meta = (AllowPrivateAccess = "true"))
	UAttackComponent* AttackComponent;

	UPROPERTY(EditDefaultsOnly, Category = HealthComponent, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

private:
	ECombatState bCombatState = ECombatState::Ready;

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	virtual void I_PlayAttackMontage(UAnimMontage* AttackMontage) override;
	virtual void I_EndAttackMontage() override;
	virtual FVector I_GetSocketLocation(const FName& SocketName) const override;

	virtual void I_ANS_TraceHit() override;

	virtual void I_ANS_BeginTraceHit() override;

	virtual void I_ANS_Combo() override;

	virtual void I_RequestAttack() override;
	virtual void I_EndHitReact() override;
	void ChangeWalkSpeed(float NewSpeed);
protected:

	UAnimMontage* GetDirectHitReactMontage(const FVector& Direction) const;

	UFUNCTION()
	void HandleHitSomething(const FHitResult& HitResult);

	UFUNCTION()
	virtual void HandleTakePointDamage(AActor* DamagedActor,
		float Damage,
		AController* InstigatedBy,
		FVector HitLocation, UPrimitiveComponent*
		FHitComponent, FName BoneName,
		FVector ShotFromDirection,
		const class UDamageType* DamageType,
		AActor* DamageCauser);

	virtual void NotifyControllerChanged() override;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	//virtual void Tick(float DeltaSecond) override;

public:
	FORCEINLINE ECombatState GetCombatState() const { return bCombatState; }
};
