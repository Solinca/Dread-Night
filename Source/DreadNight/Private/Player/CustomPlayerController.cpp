#include "Player/CustomPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <EnhancedInputSubsystems.h>


void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = Cast<APlayerCharacter>(GetPawn());
	MyPlayer->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchMoveSpeed;

	if (GetLocalPlayer())
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!MappingContext)
			{
				return;
			}
			InputSystem->AddMappingContext(MappingContext, 0);
		}

	}

	PlayerCameraManager->ViewPitchMin = -55;
	PlayerCameraManager->ViewPitchMax = 55;
}

void ACustomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCrouching(MyPlayer->GetIsCrouching(), DeltaTime);
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		for (FInputActionSetup i : IA_Setup)
		{
			EnhancedInputComponent->BindAction(i.Action, i.Event, this, i.ActionName.GetMemberName());
		}
	}
}

void ACustomPlayerController::Move(const FInputActionValue& Value)
{
	FVector MovementVector = Value.Get<FVector>();
	FRotator CameraRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	FVector RotatedVector = CameraRotation.RotateVector(MovementVector);

	GetPawn()->AddMovementInput(RotatedVector);
}


void ACustomPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D mouseInput = Value.Get<FVector2D>();
	AddYawInput(mouseInput.X * GetWorld()->GetDeltaSeconds() * CameraSensitivity);
	AddPitchInput(mouseInput.Y * GetWorld()->GetDeltaSeconds() * CameraSensitivity);
}


void ACustomPlayerController::Jump(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		MyPlayer->Jump();
	}
}

void ACustomPlayerController::Sprint(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = SprintMoveSpeed;
		MyPlayer->SetIsSprinting(true);
	}
}

void ACustomPlayerController::SprintEnd(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = BaseMoveSpeed;
		MyPlayer->SetIsSprinting(false);
	}
}

void ACustomPlayerController::Crouch(const FInputActionValue& Value)
{
	if (MyPlayer)
		MyPlayer->SetIsCrouching(Value.Get<bool>());
}

void ACustomPlayerController::CrouchEnd(const FInputActionValue& Value)
{
	MyPlayer->SetIsCrouching(false);
}

void ACustomPlayerController::UpdateCrouching(bool isCrouching, float deltatime)
{
	if (MyPlayer)
	{
		if (UCharacterMovementComponent* MovementComp = MyPlayer->GetCharacterMovement())
		{

			float CurrentSpeed = MovementComp->MaxWalkSpeed;
			float FOVInterpSpeed = 8.f;
			if (isCrouching)
			{
				MyPlayer->SetCurentCapsuleHalfHeight(FMath::FInterpTo(MyPlayer->GetCurentCapsuleHalfHeight(), 44.f, deltatime, FOVInterpSpeed));
				MyPlayer->GetCharacterMovement()->MaxWalkSpeed = CrouchMoveSpeed;
			}
			else
			{
				MyPlayer->SetCurentCapsuleHalfHeight(FMath::FInterpTo(MyPlayer->GetCurentCapsuleHalfHeight(), 88.f, deltatime, FOVInterpSpeed));

				if (!MyPlayer->GetIsSprinting())
					MyPlayer->GetCharacterMovement()->MaxWalkSpeed = BaseMoveSpeed;
			}
		}
	}
}

void ACustomPlayerController::Aim(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Aiming");
}


void ACustomPlayerController::Attack(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Attacking");
}


void ACustomPlayerController::Interact(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Stop Attacking");
}

void ACustomPlayerController::DisplayInventory(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Display Inventory");
}

void ACustomPlayerController::DisplayGlossary(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Display Glossary");
}

void ACustomPlayerController::DisplayMenu(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Display Menu");
}

void ACustomPlayerController::GoBackToPrecedentMenu(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Go Back To Precedent Menu");
}

void ACustomPlayerController::SelectedHotbar(const FInputActionValue& Value)
{
	int index = (int)Value.Get<float>();

	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar : " + FString::FromInt(index));
}

