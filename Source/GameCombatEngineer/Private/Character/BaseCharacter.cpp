#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/AttackComponent.h"
#include "DataAsset/CharacterData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Component/HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "DataAsset/InputData.h"
#include "Components/CapsuleComponent.h"
#include "Component/StaminaComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));
}
void ABaseCharacter::I_PlayAttackMontage(UAnimMontage* AttackMontage)
{
	PlayAnimMontage(AttackMontage);
}
void ABaseCharacter::I_EndAttackMontage()
{
	if (AttackComponent) {
		AttackComponent->EndAttack();
	}
}
FVector ABaseCharacter::I_GetSocketLocation(const FName& SocketName) const
{
	if (GetMesh() == nullptr) return FVector();
	return GetMesh()->GetSocketLocation(SocketName);
}
void ABaseCharacter::I_ANS_TraceHit()
{
	if (AttackComponent)
		AttackComponent->TraceHit();
}
void ABaseCharacter::I_ANS_BeginTraceHit()
{
	if (AttackComponent)
		AttackComponent->SetupTraceHit();
}
void ABaseCharacter::I_ANS_Combo()
{
	if (AttackComponent)
		AttackComponent->Combo();
}
void ABaseCharacter::I_RequestAttack()
{
	if (bCombatState == ECombatState::Ready && AttackComponent)
		AttackComponent->RequestAttack();
}
void ABaseCharacter::I_EndHitReact()
{
	bCombatState = ECombatState::Ready;
	I_EndAttackMontage();
}
void ABaseCharacter::ChangeWalkSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}
void ABaseCharacter::I_HandleAttackSuccess()
{
	if (StaminaComponent && CharacterData)
		StaminaComponent->UpdateStamina(CharacterData->CostStaminaPerAttack);
}
bool ABaseCharacter::I_IsReadyAttack() const
{
	return bCombatState == ECombatState::Ready;
}
bool ABaseCharacter::I_CheckEnoughStamina(float Cost) const
{
	if (StaminaComponent == nullptr) return false;

	return StaminaComponent->Stamina >= Cost;
}
bool ABaseCharacter::I_IsAttacking() const
{
	if (AttackComponent == nullptr) return false;
	return AttackComponent ->bIsAttacking;
}
void ABaseCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnTakePointDamage.AddDynamic(this, &ABaseCharacter::HandleTakePointDamage);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (AttackComponent)
	{
		AttackComponent->HitSomethingDelegate.BindDynamic(this, &ABaseCharacter::HandleHitSomething);
	}

	if (HealthComponent != nullptr && CharacterData != nullptr) {
		HealthComponent->SetupComponent(CharacterData);
	}

	if (StaminaComponent != nullptr) {
		StaminaComponent->SetupComponent(CharacterData);
	}
}

//void ABaseCharacter::Tick(float DeltaSecond)
//{
//	Super::Tick(DeltaSecond);
//}

UAnimMontage* ABaseCharacter::GetDirectHitReactMontage(const FVector& Direction) const
{
	if (CharacterData == nullptr) return nullptr;

	const auto dot = FVector::DotProduct(Direction, GetActorForwardVector());
	const bool bShouldUseDot = FMath::Abs(dot) > 0.5;
	if (bShouldUseDot)
	{
		if (dot > 0.0)
			return CharacterData->HitReactMontage_Back;
		else
			return CharacterData->HitReactMontage_Front;
	}
	else
	{
		auto cross = FVector::CrossProduct(Direction, GetActorForwardVector());
		if (cross.Z > 0.0)
			return CharacterData->HitReactMontage_Right;
		else
			return CharacterData->HitReactMontage_Left;
	}
	return nullptr;
}

void ABaseCharacter::HandleHitSomething(const FHitResult& HitResult)
{
	if (CharacterData == nullptr) return;

	auto Actor = HitResult.GetActor();

	if (Actor == nullptr) return;

	const auto Direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Actor->GetActorLocation());

	UGameplayStatics::ApplyPointDamage(
		Actor,
		CharacterData->Damage,
		Direction,
		HitResult,
		GetController(),
		this,
		UDamageType::StaticClass()
	);
}

void ABaseCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (HealthComponent)
		HealthComponent->UpdateHealthByDamage(Damage);
	if (HealthComponent->Health > 0.0f)
		HandleBeaten(HitLocation, ShotFromDirection);
	else
		HandleDead();
}

void ABaseCharacter::HandleBeaten(const FVector& HitLocation, const FVector& ShotFromDirection)
{
	if (CharacterData == nullptr) return;
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		CharacterData->HitImpactEffect,
		HitLocation
	);

	PlayAnimMontage(GetDirectHitReactMontage(ShotFromDirection));
	bCombatState = ECombatState::Beaten;
}

void ABaseCharacter::HandleDead()
{
	if (CharacterData == nullptr) return;

	float DeadMontageSecond = PlayAnimMontage(CharacterData->DeadMontage);
	bCombatState = ECombatState::Dead;

	if (GetCharacterMovement())
		GetCharacterMovement()->StopMovementImmediately();

	if (GetCapsuleComponent() && GetMesh()) {
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SetLifeSpan(DeadMontageSecond);
}
