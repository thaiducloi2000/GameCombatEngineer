#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAsset/InputData.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraBoom->AddLocalOffset(FVector(0.0f, 1.0f, 40.0f));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}
void ABaseCharacter::NotifyControllerChanged()
{

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
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		if (InputData != nullptr)
		{
			Subsystem->AddMappingContext(InputData->DefaultMappingContext, 0);
		}
	}
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
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
		GEngine->AddOnScreenDebugMessage(
			/* Key */ -1,
			/* TimeToDisplay */ 5.0f,
			/* Color */ FColor::Green,
			/* Message */ TEXT("Tehee Move")
		);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(
		/* Key */ -1,
		/* TimeToDisplay */ 5.0f,
		/* Color */ FColor::Green,
		/* Message */ TEXT("Tehee Look")
	);
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABaseCharacter::Attack(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(
		/* Key */ -1,
		/* TimeToDisplay */ 5.0f,
		/* Color */ FColor::Green,
		/* Message */ TEXT("Tehee Attack")
	);
}

void ABaseCharacter::Run(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(
		/* Key */ -1,
		/* TimeToDisplay */ 5.0f,
		/* Color */ FColor::Green,
		/* Message */ TEXT("Tehee Run")
	);
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
}

void ABaseCharacter::StopRun(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GEngine->AddOnScreenDebugMessage(
		/* Key */ -1,
		/* TimeToDisplay */ 5.0f,
		/* Color */ FColor::Green,
		/* Message */ TEXT("Tehee StopRun")
	);
}
