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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputData* InputData;

protected:
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
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
protected:

	UAnimMontage* GetDirectHitReactMontage(const FVector& Direction) const;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/* Called for attack input*/
	void Attack(const FInputActionValue& Value);

	/* Called for run input*/
	void Run(const FInputActionValue& Value);

	/* Called for stop run input*/
	void StopRun(const FInputActionValue& Value);

	UFUNCTION()
	void HandleHitSomething(const FHitResult& HitResult);

	UFUNCTION()
	void HandleTakePointDamage(AActor* DamagedActor,
		float Damage,
		AController* InstigatedBy,
		FVector HitLocation, UPrimitiveComponent*
		FHitComponent, FName BoneName,
		FVector ShotFromDirection,
		const class UDamageType* DamageType,
		AActor* DamageCauser);

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	//virtual void Tick(float DeltaSecond) override;

	void SetupCharacterData();

public:
	FORCEINLINE ECombatState GetCombatState() const { return bCombatState; }
};
