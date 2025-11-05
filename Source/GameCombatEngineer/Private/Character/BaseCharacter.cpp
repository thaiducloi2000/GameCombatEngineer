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
	if (CharacterData == nullptr) return;

	if (HealthComponent)
		HealthComponent->UpdateHealthByDamage(Damage);

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		CharacterData->HitImpactEffect,
		HitLocation
	);

	PlayAnimMontage(GetDirectHitReactMontage(ShotFromDirection));
	bCombatState = ECombatState::Beaten;
}
