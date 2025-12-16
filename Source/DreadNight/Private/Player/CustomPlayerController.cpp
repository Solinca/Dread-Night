#include "Player/CustomPlayerController.h"
#include "Global/BaseLevelWorldSettings.h"
#include "Global/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widgets/Inventory.h"
#include "UI/Widgets/PauseMenu.h"
#include "UserWidgets/OptionsWidget.h"
#include "UI/Widgets/PlayerHud.h"
#include "Actors/Building.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "InteractableSystem/Subsystems/InteractableSubsystem.h"

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PauseCounter = 0;

	if (!MappingContextBase)
	{
		return;
	}

	if (GetLocalPlayer())
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->AddMappingContext(MappingContextBase, 0);
		}
	}

	MyPlayer = Cast<APlayerCharacter>(GetPawn());

	MyPlayer->GetCharacterMovement()->MaxWalkSpeedCrouched = PlayerData->CrouchMoveSpeed;

	MyPlayer->GetHealthComponent()->OnDeath.AddDynamic(this, &ThisClass::ShowGameOver);

	if (PlayerData->HotbarInventoryWidgetClass)
	{
		HotbarInventoryWidget = CreateWidget<UInventory>(this, PlayerData->HotbarInventoryWidgetClass);
		HotbarInventoryWidget->BindToInventory(MyPlayer->GetHotbarInventoryComponent());
		HotbarInventoryWidget->BindTargetInventory(MyPlayer->GetInventoryComponent());
		HotbarInventoryWidget->AddToViewport();
	}
	PlayerCameraManager->ViewPitchMin = PlayerData->ViewPitch.X;

	PlayerCameraManager->ViewPitchMax = PlayerData->ViewPitch.Y;

	SetInputMode(FInputModeGameOnly());

	HUDWidget = CreateWidget<UPlayerHud>(this, PlayerData->PlayerHudClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
		BindUIEvents();
	}

	ObjectPlacementQueryParams.bTraceComplex = true;
	ObjectPlacementQueryParams.AddIgnoredActor(GetPawn());
}

void ACustomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCrouching(DeltaTime);
	UpdateObjectPlacement();
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

void ACustomPlayerController::UpdateGamePauseState()
{
	const bool bShouldPause = (PauseCounter > 0);

	UGameplayStatics::SetGamePaused(GetWorld(), bShouldPause);
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

	AddYawInput(mouseInput.X * GetWorld()->GetDeltaSeconds() * PlayerData->CameraSensitivity);

	AddPitchInput(mouseInput.Y * GetWorld()->GetDeltaSeconds() * PlayerData->CameraSensitivity);
}

void ACustomPlayerController::Jump(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();

		if (StaminaComponent->GetCurrentStamina() > 0.f && MyPlayer->CanJump())
		{
			MyPlayer->Jump();

			StaminaComponent->RemoveStamina(PlayerData->JumpStaminaCost);

			StaminaComponent->SetCanRegen(false);

			// START REGEN STAMINA
			GetWorldTimerManager().SetTimer(StaminaComponent->CoolDownTimer,
				[=] {StaminaComponent->SetCanRegen(true); },
				StaminaComponent->GetRegenCoolDown(), false
			);

			MyPlayer->GetConditionStateComponent()->RemoveHungerValue(PlayerData->HungerJumpCost);
		}
	}
}

void ACustomPlayerController::Sprint(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();

		if (StaminaComponent->GetCurrentStamina() > 0.f)
		{
			MyPlayer->GetCharacterMovement()->MaxWalkSpeed = PlayerData->SprintMoveSpeed;

			MyPlayer->SetIsSprinting(true);

			StaminaComponent->SetCanRegen(false);

			StaminaComponent->RemoveStamina(PlayerData->SprintStaminaCost * GetWorld()->GetDeltaSeconds());

			MyPlayer->GetConditionStateComponent()->RemoveHungerValue(PlayerData->HungerSprintCost * GetWorld()->GetDeltaSeconds());
		}
		else
		{
			SprintEnd(Value);
		}
	}
}

void ACustomPlayerController::SprintEnd(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		MyPlayer->GetCharacterMovement()->MaxWalkSpeed = PlayerData->BaseMoveSpeed;

		MyPlayer->SetIsSprinting(false);

		UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();

		// START REGEN STAMINA
		GetWorldTimerManager().SetTimer(StaminaComponent->CoolDownTimer,
			[=] {StaminaComponent->SetCanRegen(true); },
			StaminaComponent->GetRegenCoolDown(), false
		);
	}
}

void ACustomPlayerController::Crouch(const FInputActionValue& Value)
{
	if (MyPlayer)
	{
		MyPlayer->SetIsCrouching(Value.Get<bool>());
	}
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
		{
			MyPlayer->GetCharacterMovement()->MaxWalkSpeed = PlayerData->CrouchMoveSpeed;
		}
		else
		{
			if (!MyPlayer->GetIsSprinting())
			{
				MyPlayer->GetCharacterMovement()->MaxWalkSpeed = PlayerData->BaseMoveSpeed;
			}
		}
	}
}

void ACustomPlayerController::UpdateObjectPlacement()
{
	if (CreatedBuilding)
	{
		FHitResult Hit;

		if (GetWorld()->LineTraceSingleByChannel(
			Hit,
			PlayerCameraManager->GetCameraLocation(),
			PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetCameraRotation().Vector() * ObjectPlacementRange),
			ECC_WorldStatic,
			ObjectPlacementQueryParams) &&
			!Hit.GetActor()->IsA(ACharacter::StaticClass()))
		{
			CreatedBuilding->SetActorLocation(Hit.ImpactPoint);
		}
		else
		{
			CreatedBuilding->SetActorLocation(PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetCameraRotation().Vector() * ObjectPlacementRange));
		}

		CreatedBuilding->CheckValidPlacement();
	}
}

void ACustomPlayerController::Aim(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Aiming");
}

void ACustomPlayerController::Attack(const FInputActionValue& Value)
{
	USwordCombatComponent* SwordCombatComponent = MyPlayer->GetSwordCombatComponent();
	UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();

	if (!SwordCombatComponent->GetIsAttacking() && StaminaComponent->GetCurrentStamina() > 0.f)
	{
		SwordCombatComponent->Attack();

		StaminaComponent->RemoveStamina(PlayerData->AttackStaminaCost);

		StaminaComponent->SetCanRegen(false);

		// START REGEN STAMINA
		GetWorldTimerManager().SetTimer(StaminaComponent->CoolDownTimer,
			[=] {StaminaComponent->SetCanRegen(true); },
			StaminaComponent->GetRegenCoolDown(), false
		);

		MyPlayer->GetConditionStateComponent()->RemoveHungerValue(PlayerData->HungerAttackCost);
	}
}


void ACustomPlayerController::Interact(const FInputActionValue& Value)
{
	TObjectPtr<UInteractableSubsystem> Subsystem = GetWorld()->GetSubsystem<UInteractableSubsystem>();
	if (Subsystem->TryInteract())
		Subsystem->RequestInteraction(Subsystem->GetLastFocusedActor(), MyPlayer);
}

void ACustomPlayerController::DisplayInventory(const FInputActionValue& Value)
{
	if (!PlayerData->InventoryWidgetClass)
		return;

	InventoryWidget = CreateWidget<UInventory>(this, PlayerData->InventoryWidgetClass);
	InventoryWidget->BindToInventory(MyPlayer->GetInventoryComponent());
	InventoryWidget->BindTargetInventory(MyPlayer->GetHotbarInventoryComponent());
	FVector2D WindowSize = GEngine->GameViewport->Viewport->GetSizeXY();
	InventoryWidget->SetDesiredSizeInViewport(FVector2D(600, 600));
	InventoryWidget->SetPositionInViewport(FVector2D(WindowSize.X / 2 - 300, WindowSize.Y / 2 - 300));
	SetShowMouseCursor(true);

	PushNewMenu(InventoryWidget, false, [this]
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);

			if (UInventory* TempInventory = Cast<UInventory>(InventoryWidget))
			{
				TempInventory->RemoveItemAction();
			}
			if (UInventory* TempHotBar = Cast<UInventory>(HotbarInventoryWidget))
			{
				TempHotBar->RemoveItemAction();
			}
		});
}

void ACustomPlayerController::DisplayGlossary(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Display Glossary");
}

void ACustomPlayerController::DisplayMenu(const FInputActionValue& Value)
{
	if (!PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget<UPauseMenu>(this, PlayerData->PauseMenuClass);

		PauseMenuWidget->OnResume.AddDynamic(this, &ThisClass::ResumeGame);

		PauseMenuWidget->OnOptions.AddDynamic(this, &ThisClass::AccessOptions);

		PauseMenuWidget->OnQuitToMenu.AddDynamic(this, &ThisClass::GoBackToMenu);

		PauseMenuWidget->OnQuitToDesktop.AddDynamic(this, &ThisClass::LeaveGame);


		PushNewMenu(PauseMenuWidget, true, [this]
			{
				PauseMenuWidget = nullptr;
			});
	}
}

void ACustomPlayerController::GoBackToPrecedentMenu(const FInputActionValue& Value)
{
	PopLastMenu();
}

void ACustomPlayerController::SelectedHotbar(const FInputActionValue& Value)
{
	int index = (int)Value.Get<float>();

	if (index == -1)
		index = 0;

	MyPlayer->GetInventoryComponent()->UseItemAt(index);
}

void ACustomPlayerController::SaveGame()
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->Save(GetWorld());
	}
}

void ACustomPlayerController::LoadGame()
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->Load(GetWorld());
	}
}

void ACustomPlayerController::GoBackToMenu()
{
	SaveGame();

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), PlayerData->MainMenuLevel);
}

void ACustomPlayerController::PlaceObject(const FInputActionValue& Value)
{
	if (CreatedBuilding)
	{
		if (CreatedBuilding->CheckValidPlacement())
		{
			CreatedBuilding->PlaceBuilding();
			CreatedBuildings.Add(CreatedBuilding);
		}
		else
		{
			return;
		}
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CreatedBuilding = GetWorld()->SpawnActor<ABuilding>(
		DebugBuilding,
		PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetCameraRotation().Vector() * ObjectPlacementRange),
		FRotator::ZeroRotator,
		SpawnParams
	);

	ObjectPlacementQueryParams.AddIgnoredActor(CreatedBuilding);
}

void ACustomPlayerController::RotateObject(const FInputActionValue& Value)
{
	if (!CreatedBuilding) return;

	float Axis = Value.Get<float>();

	if (Axis != 0.f)
	{
		CreatedBuilding->AddActorLocalRotation(
			FRotator(0.f,
				Axis * BuildingRotationSpeed * GetWorld()->GetDeltaSeconds(),
				0.f)
		);
	}
}

void ACustomPlayerController::PopLastMenu()
{
	if (MenuStack.IsEmpty() || !MenuStack.Last().bCanBeQuit)
	{
		return;
	}

	FStackedMenu LastMenu = MenuStack.Pop();

	if (LastMenu.Widget)
	{
		LastMenu.Widget->RemoveFromParent();

		LastMenu.OnCloseAction();

		if (LastMenu.bTriggerPause)
		{
			PauseCounter--;
		}
	}

	if (MenuStack.IsEmpty())
	{
		PauseCounter = 0;

		if (GetLocalPlayer())
		{
			if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (MappingContextBase)
				{
					InputSystem->ClearAllMappings();

					InputSystem->AddMappingContext(MappingContextBase, 0);
				}
			}
		}

		SetInputMode(FInputModeGameOnly());

		SetShowMouseCursor(false);

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
	else
	{
		FStackedMenu& NewTopMenu = MenuStack.Last();

		if (NewTopMenu.Widget)
		{
			NewTopMenu.Widget->SetVisibility(ESlateVisibility::Visible);
		}

		UpdateGamePauseState();
	}
}

void ACustomPlayerController::ResumeGame()
{
	PopLastMenu();
}

void ACustomPlayerController::PauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ACustomPlayerController::AccessOptions()
{
	if (!OptionsWidget)
	{
		OptionsWidget = CreateWidget<UOptionsWidget>(this, PlayerData->OptionsClass);

		OptionsWidget->OnReturn.AddDynamic(this, &ThisClass::QuitOptions);

		PushNewMenu(OptionsWidget, true, [this]
			{
				OptionsWidget = nullptr;
			});
	}
}

void ACustomPlayerController::QuitOptions()
{
	PopLastMenu();
}

void ACustomPlayerController::LeaveGame()
{
	SaveGame();

	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}

void ACustomPlayerController::ShowGameOver()
{
	TObjectPtr<UUserWidget> WidgetGameOver = CreateWidget<UUserWidget>(this, PlayerData->GameOverClass);

	PushNewMenu(WidgetGameOver, true, []() {}, false);

	UGameplayStatics::PlaySound2D(this, PlayerData->GameOverSound);
}

void ACustomPlayerController::BindUIEvents()
{
	MyPlayer->GetHealthComponent()->OnHealthChanged.AddDynamic(HUDWidget, &UPlayerHud::UpdateHealthBar);
	MyPlayer->GetStaminaComponent()->OnStaminaChanged.AddDynamic(HUDWidget, &UPlayerHud::UpdateStaminaBar);
	MyPlayer->GetManaComponent()->OnManaChanged.AddDynamic(HUDWidget, &UPlayerHud::UpdateManaBar);
	MyPlayer->GetConditionStateComponent()->OnHungerChanged.AddDynamic(HUDWidget, &UPlayerHud::UpdateHungerRadialBarImage);
	MyPlayer->GetConditionStateComponent()->OnThirstChanged.AddDynamic(HUDWidget, &UPlayerHud::UpdateThirstRadialBarImage);
	MyPlayer->GetConditionStateComponent()->OnSanityChanged.AddDynamic(HUDWidget, &UPlayerHud::UpdateFearRadialBarImage);
}
