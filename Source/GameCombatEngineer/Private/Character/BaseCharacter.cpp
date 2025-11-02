#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAsset/InputData.h"
#include "Component/AttackComponent.h"
#include "DataAsset/CharacterData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));
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
void ABaseCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputData != nullptr)
			{
				Subsystem->AddMappingContext(InputData->DefaultMappingContext, 0);
			}
		}
	}
}

void ABaseCharacter::SetupCharacterData()
{
	if (CharacterData)
	{
		// Configure character movement
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);/*CharacterData->RotationRate*/; // ...at this rotation rate

		// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
		// instead of recompiling to adjust them
		GetCharacterMovement()->JumpZVelocity = CharacterData->JumpZVelocity;
		GetCharacterMovement()->AirControl = CharacterData->AirControl;
		GetCharacterMovement()->MaxWalkSpeed = CharacterData->MaxWalkSpeed;
		GetCharacterMovement()->MinAnalogWalkSpeed = CharacterData->MinAnalogWalkSpeed;
		GetCharacterMovement()->BrakingDecelerationWalking = CharacterData->BrakingDecelerationWalking;
		GetCharacterMovement()->BrakingDecelerationFalling = CharacterData->BrakingDecelerationFalling;

		CameraBoom->TargetArmLength = CharacterData->TargetArmLength;
		CameraBoom->AddLocalOffset(CharacterData->LocalOffset);
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (InputData != nullptr)
		{
			// Jumping
			EnhancedInputComponent->BindAction(InputData->JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(InputData->JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			// Moving
			EnhancedInputComponent->BindAction(InputData->MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

			// Looking
			EnhancedInputComponent->BindAction(InputData->LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);

			// Run
			EnhancedInputComponent->BindAction(InputData->RunAction, ETriggerEvent::Started, this, &ABaseCharacter::Run);
			EnhancedInputComponent->BindAction(InputData->RunAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopRun);

			// Attack
			EnhancedInputComponent->BindAction(InputData->AttackAction, ETriggerEvent::Started, this, &ABaseCharacter::Attack);
		}
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnTakePointDamage.AddDynamic(this, &ABaseCharacter::HandleTakePointDamage);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetupCharacterData();
	if (AttackComponent)
	{
		AttackComponent->HitSomethingDelegate.BindDynamic(this, &ABaseCharacter::HandleHitSomething);
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

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector ForwardDirection = YawRotation.RotateVector(FVector::ForwardVector);

		// get right vector 
		//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector RightDirection = YawRotation.RotateVector(FVector::RightVector);
		// add movement 
		if (MovementVector.Y != 0.f)
		{
			AddMovementInput(ForwardDirection, MovementVector.Y);
		}

		if (MovementVector.X != 0.f)
		{
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABaseCharacter::Attack(const FInputActionValue& Value)
{
	if (AttackComponent) {
		AttackComponent->RequestAttack();
	}
}

void ABaseCharacter::Run(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
}

void ABaseCharacter::StopRun(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void ABaseCharacter::HandleHitSomething(const FHitResult& HitResult)
{
	if (CharacterData == nullptr) return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Handle Hit Something"));

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
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Handle Take Point Damage"));
	if (CharacterData)
	{
		PlayAnimMontage(GetDirectHitReactMontage(ShotFromDirection));
		bCombatState = ECombatState::Beaten;
	}
}
