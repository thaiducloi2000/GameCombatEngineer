#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAsset/InputData.h"
#include "DataAsset/CharacterData.h"

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
			EnhancedInputComponent->BindAction(InputData->AttackAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Attack);
		}
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupCharacterData();
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FString InputText = FString::Printf(TEXT("Move Input: X=%.2f | Y=%.2f"), MovementVector.X, MovementVector.Y);
		GEngine->AddOnScreenDebugMessage(-1, 0.05f, FColor::Cyan, InputText);
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
}

void ABaseCharacter::Run(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
}

void ABaseCharacter::StopRun(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}
