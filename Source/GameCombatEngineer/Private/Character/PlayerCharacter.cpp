// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Widget/PlayerWidget.h"
#include "DataAsset/CharacterData.h"
#include "Component/HealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Component/AttackComponent.h"
#include "DataAsset/InputData.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	bPlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);

	if (bPlayerWidget != nullptr && HealthComponent != nullptr) {
		bPlayerWidget->AddToViewport();
		bPlayerWidget->UpdateHealthBar_Player(HealthComponent->Health, HealthComponent->MaxHealth);
		bPlayerWidget->HideEnemyStat();
	}
}

void APlayerCharacter::I_EnterCombat(float Health_Target, float MaxHealth_Target)
{
	if (bPlayerWidget != nullptr && HealthComponent != nullptr) {
		bPlayerWidget->ShowEnemyStat();
		bPlayerWidget->UpdateHealthBar_Enemy(Health_Target, MaxHealth_Target);
	}
	if (CharacterData)
		ChangeWalkSpeed(CharacterData->CombatSpeed);
}

void APlayerCharacter::I_ExitCombat()
{
	if (bPlayerWidget != nullptr && HealthComponent != nullptr) {
		bPlayerWidget->HideEnemyStat();
	}
}

void APlayerCharacter::I_HitTarget(float Health_Target, float MaxHealth_Target)
{
	if (bPlayerWidget != nullptr && HealthComponent != nullptr) {
		bPlayerWidget->UpdateHealthBar_Enemy(Health_Target, MaxHealth_Target);
	}
}

void APlayerCharacter::I_HandleTargetDestroy()
{
	if (bPlayerWidget != nullptr) {
		bPlayerWidget->HideEnemyStat();
	}
}


void APlayerCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	Super::HandleTakePointDamage(DamagedActor,
		Damage,
		InstigatedBy,
		HitLocation,
		FHitComponent,
		BoneName,
		ShotFromDirection,
		DamageType,
		DamageCauser);
	if (bPlayerWidget && HealthComponent)
		bPlayerWidget->UpdateHealthBar_Player(HealthComponent->Health, HealthComponent->MaxHealth);
}

void APlayerCharacter::HandleDead()
{
	Super::HandleDead();
	if (bPlayerWidget != nullptr) {
		bPlayerWidget->RemoveFromParent();
	}

	auto PlayerControler = Cast<APlayerController>(GetController());
	if (PlayerControler)
		DisableInput(PlayerControler);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
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

void APlayerCharacter::Look(const FInputActionValue& Value)
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

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
	I_RequestAttack();
	//if (AttackComponent) {
	//	AttackComponent->RequestAttack();
	//}
}

void APlayerCharacter::ExitCombat(const FInputActionValue& Value)
{
	if (bPlayerWidget) {
		bPlayerWidget->HideEnemyStat();
	}

	if (I_OnExitCombat.IsBound() == true)
		I_OnExitCombat.Execute();
}

//void APlayerCharacter::StopRun(const FInputActionValue& Value)
//{
//	if (CharacterData)
//		ChangeWalkSpeed(CharacterData->MaxWalkSpeed);
//}

void APlayerCharacter::SetupCharacterData()
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

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetupCharacterData();
}

void APlayerCharacter::NotifyControllerChanged()
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

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (InputData != nullptr)
		{
			// Jumping
			//EnhancedInputComponent->BindAction(InputData->JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			//EnhancedInputComponent->BindAction(InputData->JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			// Moving
			EnhancedInputComponent->BindAction(InputData->MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

			// Looking
			EnhancedInputComponent->BindAction(InputData->LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

			// Run
			EnhancedInputComponent->BindAction(InputData->RunAction, ETriggerEvent::Started, this, &APlayerCharacter::ExitCombat);
			//EnhancedInputComponent->BindAction(InputData->RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopRun);

			// Attack
			EnhancedInputComponent->BindAction(InputData->AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
		}
	}
}

