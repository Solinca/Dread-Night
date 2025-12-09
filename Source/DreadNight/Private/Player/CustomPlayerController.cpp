#include "Player/CustomPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/PauseMenu.h"
#include <EnhancedInputSubsystems.h>

#include "Global/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = Cast<APlayerCharacter>(GetPawn());
	MyPlayer->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchMoveSpeed;

	if (GetLocalPlayer())
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!MappingContextBase)
			{
				return;
			}
			InputSystem->AddMappingContext(MappingContextBase, 0);
		}

	}
	PlayerCameraManager->ViewPitchMin = ViewPitch.X;
	PlayerCameraManager->ViewPitchMax = ViewPitch.Y;
}

void ACustomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCrouching(DeltaTime);
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
		for (FInputActionSetup i : IA_SetupMenu)
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
		UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();
		StaminaComponent->RemoveStamina(JumpStaminaCost);
		StaminaComponent->SetCanRegen(false);

		//START REGEN STAMINA
		GetWorldTimerManager().SetTimer(
			StaminaComponent->CoolDownTimer,
			[=] {StaminaComponent->SetCanRegen(true); },
			StaminaComponent->GetRegenCoolDown(),
			false
		);

		MyPlayer->GetConditionStateComponent()->RemoveHungerValue(HungerJumpCost);
	}
}

void ACustomPlayerController::Sprint(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = SprintMoveSpeed;
		MyPlayer->SetIsSprinting(true);
		StaminaComponent->SetCanRegen(false);

		StaminaComponent->RemoveStamina(SprintStaminaCost * GetWorld()->GetDeltaSeconds());
		MyPlayer->GetConditionStateComponent()->RemoveHungerValue(HungerSprintCost * GetWorld()->GetDeltaSeconds());
	}
}

void ACustomPlayerController::SprintEnd(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = BaseMoveSpeed;
		MyPlayer->SetIsSprinting(false);

		UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();
		//START REGEN STAMINA
		GetWorldTimerManager().SetTimer(
			StaminaComponent->CoolDownTimer,
			[=] {StaminaComponent->SetCanRegen(true); },
			StaminaComponent->GetRegenCoolDown(),
			false
		);
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

void ACustomPlayerController::UpdateCrouching(float deltatime)
{
	if (MyPlayer)
	{
		MyPlayer->UpdateCrouching(deltatime);

		if (MyPlayer->GetIsCrouching())
			MyPlayer->GetCharacterMovement()->MaxWalkSpeed = CrouchMoveSpeed;
		else
		{
			if (!MyPlayer->GetIsSprinting())
				MyPlayer->GetCharacterMovement()->MaxWalkSpeed = BaseMoveSpeed;
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

	UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();

	StaminaComponent->RemoveStamina(AttackStaminaCost);

	StaminaComponent->SetCanRegen(false);

	//START REGEN STAMINA
	GetWorldTimerManager().SetTimer(
		StaminaComponent->CoolDownTimer,
		[=] {StaminaComponent->SetCanRegen(true); },
		StaminaComponent->GetRegenCoolDown(),
		false
	);

	MyPlayer->GetConditionStateComponent()->RemoveHungerValue(HungerAttackCost);
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
	if (!PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget<UPauseMenu>(this, PauseMenuClass);
		PauseMenuWidget->OnResume.AddDynamic(this, &ThisClass::ResumeGame);
		PauseMenuWidget->OnQuitToMenu.AddDynamic(this, &ThisClass::GoBackToMenu);
		PauseMenuWidget->OnQuitToDesktop.AddDynamic(this, &ThisClass::LeaveGame);
		PauseGame();

		PushNewMenu(PauseMenuWidget, [this]{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			;});
	}
}

void ACustomPlayerController::GoBackToPrecedentMenu(const FInputActionValue& Value)
{
	//TODO IF MENU QUEUE == LAST (if user is in the last menu before going back to the game)
	PopLastMenu();	
}

void ACustomPlayerController::SelectedHotbar(const FInputActionValue& Value)
{
	int index = (int)Value.Get<float>();

	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Hotbar : " + FString::FromInt(index));
}

void ACustomPlayerController::SaveGame()
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->Save(GetWorld());
	}
	
}

void ACustomPlayerController::GoBackToMenu()
{
	SaveGame();
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), WorldMenu);	
}


void ACustomPlayerController::PopLastMenu()
{
	if (MenuList.IsEmpty())
		return;

	auto& LastMenu = *MenuList.Peek();

	LastMenu.Key->RemoveFromParent();
	LastMenu.Value();
	MenuList.Pop();
	
	if (MenuList.IsEmpty())
	{
		if (GetLocalPlayer())
		{
			if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (!MappingContextBase)
				{
					return;
				}
				InputSystem->ClearAllMappings();
				InputSystem->AddMappingContext(MappingContextBase, 0); 
				SetInputMode(FInputModeGameOnly());
			}
		}
		SetShowMouseCursor(false);
	}
}

void ACustomPlayerController::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	PopLastMenu();
}

void ACustomPlayerController::PauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ACustomPlayerController::LeaveGame()
{
	SaveGame();
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}
 