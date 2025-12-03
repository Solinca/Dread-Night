// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CustomPlayerController.h"
#include <EnhancedInputSubsystems.h>

ACustomPlayerController::ACustomPlayerController()
{
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = Cast<APlayerCharacter>(GetPawn());

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

}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		for (FInputActionSetup i : IA_Setup)
		{
			EnhancedInputComponent->BindAction(i.Action, i.Event, this, i.Name);
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

void ACustomPlayerController::MoveEnd(const FInputActionValue& Value)
{

}

void ACustomPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D mouseInput = Value.Get<FVector2D>();
	AddYawInput(mouseInput.X * GetWorld()->GetDeltaSeconds() * CameraSensibility);
	AddPitchInput(mouseInput.Y * GetWorld()->GetDeltaSeconds() * CameraSensibility);
}

void ACustomPlayerController::LookEnd(const FInputActionValue& Value)
{

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

}

void ACustomPlayerController::SprintEnd(const FInputActionValue& Value)
{

}

void ACustomPlayerController::Crouch(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		MyPlayer->Crouch();
	}
}

void ACustomPlayerController::CrouchEnd(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		MyPlayer->UnCrouch();
	}
}

void ACustomPlayerController::Aim(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Aiming");
}

void ACustomPlayerController::AimEnd(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Stop Aiming");
}

void ACustomPlayerController::Attack(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Attacking");
}

void ACustomPlayerController::AttackEnd(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Stop Attacking");
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

	switch (index)
	{
	case 0:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 1");
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 2");
		break;
	case 2:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 3");
		break;
	case 3:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 4");
		break;
	case 4:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 5");
		break;
	case 5:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 6");
		break;
	case 6:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 7");
		break;
	case 7:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 8");
		break;
	case 8:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 9");
		break;
	case 9:
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar 0");
		break;
	default:
		break;
	}
}

