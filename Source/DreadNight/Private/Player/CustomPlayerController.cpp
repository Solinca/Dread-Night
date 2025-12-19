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
#include "Actors/BuildingStation.h"
#include "GameFramework/Actor.h"
#include "UI/Widgets/Map/MapWidget.h"
#include "InteractableSystem/Subsystems/InteractableSubsystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Buildings/Chest.h"
#include "Items/Data/ItemGameplayTag.h"
#include "UI/Widgets/Glossary.h"
#include "UI/Widgets/HotBar.h"
#include "Global/MyGameUserSettings.h"

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
	
	MyPlayer->GetArmorComponent()->OnArmorEquipped.AddDynamic(this, &ThisClass::ChangeArmorUI);
	
	PlayerCameraManager->ViewPitchMin = PlayerData->ViewPitch.X;

	PlayerCameraManager->ViewPitchMax = PlayerData->ViewPitch.Y;

	ObjectPlacementQueryParams.bTraceComplex = true;
	ObjectPlacementQueryParams.AddIgnoredActor(GetPawn());

	MySettings = Cast<UMyGameUserSettings>(GEngine->GetGameUserSettings());
	
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	MyGameInstance->OnPCGEndGeneration.AddDynamic(this, &ThisClass::AddPlayerUIToViewport);
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

	AddYawInput(mouseInput.X * GetWorld()->GetDeltaSeconds() * MySettings->GetPlayerCameraSensitivity());

	AddPitchInput(mouseInput.Y * GetWorld()->GetDeltaSeconds() * MySettings->GetPlayerCameraSensitivity());
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
	if (MyPlayer->GetEquippedObjectTag().ToString().Contains("Item.Weapon.Bow"))
	{
		MyPlayer->GetBowCombatComponent()->SetAiming(true);
	}
}

void ACustomPlayerController::StopAim(const FInputActionValue& Value)
{
	if (MyPlayer->GetEquippedObjectTag().ToString().Contains("Item.Weapon.Bow"))
	{
		MyPlayer->GetBowCombatComponent()->SetAiming(false);
	}
}

void ACustomPlayerController::Attack(const FInputActionValue& Value)
{
	USwordCombatComponent* SwordCombatComponent = MyPlayer->GetSwordCombatComponent();
	UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();
	UBowCombatComponent* BowCombatComponent = MyPlayer->GetBowCombatComponent();

	bool AttackExecuted = false;
	if (!CreatedBuilding)
	{
		if (MyPlayer->GetEquippedObjectTag().ToString().Contains("Item.Weapon.Sword"))
		{
			if (!SwordCombatComponent->GetIsAttacking() && StaminaComponent->GetCurrentStamina() > 0.f)
			{
				SwordCombatComponent->Attack();
				StaminaComponent->RemoveStamina(PlayerData->AttackStaminaCost);

				AttackExecuted = true;
			}
		}
		else if (MyPlayer->GetEquippedObjectTag().ToString().Contains("Item.Weapon.Bow"))
		{
			if (BowCombatComponent->IsAiming() && BowCombatComponent->CanShoot() && StaminaComponent->GetCurrentStamina() > 0.f)
			{
				BowCombatComponent->Shoot();
				StaminaComponent->RemoveStamina(PlayerData->AttackStaminaCost);

				AttackExecuted = true;
			}
		}

		if (AttackExecuted)
		{
			StaminaComponent->SetCanRegen(false);

			// START REGEN STAMINA
			GetWorldTimerManager().SetTimer(StaminaComponent->CoolDownTimer,
				[=] {StaminaComponent->SetCanRegen(true); },
				StaminaComponent->GetRegenCoolDown(), false
			);

			MyPlayer->GetConditionStateComponent()->RemoveHungerValue(PlayerData->HungerAttackCost);
		}
	}
}


void ACustomPlayerController::Interact(const FInputActionValue& Value)
{

	TObjectPtr<UInteractableSubsystem> Subsystem = GetWorld()->GetSubsystem<UInteractableSubsystem>();

	if (Subsystem->TryInteract())
	{
		Subsystem->RequestInteraction(Subsystem->GetLastFocusedActor(), MyPlayer);
		
		if (ABuildingStation* Crafting = Cast<ABuildingStation>(Subsystem->GetLastFocusedActor()))
		{
			GlossaryWidget = CreateWidget<UGlossary>(this, Crafting->GetCraftingComponent()->GetWidget());
			PushNewMenu(GlossaryWidget, false);
		}
		if (AChest* Chest = Cast<AChest>(Subsystem->GetLastFocusedActor()))
		{
			DisplayOtherInventory(Chest->GetComponentByClass<UInventoryComponent>());
		}
	}
}

void ACustomPlayerController::DisplayInventory(const FInputActionValue& Value)
{
	if (!PlayerData->InventoryWidgetClass)
		return;

	InventoryWidget = CreateWidget<UInventory>(this, PlayerData->InventoryWidgetClass);
	InventoryWidget->BindToInventory(MyPlayer->GetInventoryComponent());
	InventoryWidget->BindTargetInventory(MyPlayer->GetHotbarInventoryComponent());
	
	if (UHotBar* HotBar = Cast<UHotBar>(HotbarInventoryWidget))
	{
		HotBar->SetArmorImagesVisibility(ESlateVisibility::Visible);
	}
	
	FVector2D WindowSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	float WidgetOffsetX = 35.f;
	float WidgetOffsetY = 75.f;
	InventoryWidget->SetDesiredSizeInViewport(FVector2D(600,600));
	InventoryWidget->SetPositionInViewport(FVector2D(WindowSize.X / 2 - (InventoryWidget->GetInventoryWrapBox()->GetWrapSize() / 2) + WidgetOffsetX,
											WindowSize.Y / 2 - (InventoryWidget->GetInventoryWrapBox()->GetWrapSize() / 2 ) + WidgetOffsetY));
	SetShowMouseCursor(true);

	PushNewMenu(InventoryWidget, false, [this]
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);

			if (UInventory* TempInventory = Cast<UInventory>(InventoryWidget))
			{
				TempInventory->RemoveItemAction();
				TempInventory->OnItemInfoRemoved();
			}
			if (UHotBar* TempHotBar = Cast<UHotBar>(HotbarInventoryWidget))
			{
				TempHotBar->RemoveItemAction();
				TempHotBar->OnItemInfoRemoved();
				TempHotBar->SetArmorImagesVisibility(ESlateVisibility::Hidden);
			}
		});
}

void ACustomPlayerController::DisplayOtherInventory(UInventoryComponent* OtherInventory)
{
	if (!PlayerData->InventoryWidgetClass)
		return;
	
	TObjectPtr<UInventoryBackground> BackgroundWidget = CreateWidget<UInventoryBackground>(this, PlayerData->OtherInventoryBackgroundWidgetClass);
	
	InventoryWidget = CreateWidget<UInventory>(this, PlayerData->InventoryWidgetClass);
	InventoryWidget->BindToInventory(MyPlayer->GetInventoryComponent());
	InventoryWidget->BindTargetInventory(OtherInventory);
	
	OtherInventoryWidget = CreateWidget<UInventory>(this, PlayerData->InventoryWidgetClass);
	OtherInventoryWidget->BindToInventory(OtherInventory);
	OtherInventoryWidget->BindTargetInventory(MyPlayer->GetInventoryComponent());
	
	BackgroundWidget->AddChildToInventoryCanvas(InventoryWidget);
	BackgroundWidget->AddChildToOtherInventoryCanvas(OtherInventoryWidget);
	
	InventoryWidget->GetInventoryWrapBox()->SetExplicitWrapSize(true);
	OtherInventoryWidget->GetInventoryWrapBox()->SetExplicitWrapSize(true);
	
	SetShowMouseCursor(true);
	PushNewMenu(BackgroundWidget, false, [this]
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
				
		if (UInventory* TempInventory = Cast<UInventory>(InventoryWidget))
		{
			TempInventory->RemoveItemAction();
			TempInventory->OnItemInfoRemoved();
		}
		if (UInventory* TempOtherInventory = Cast<UInventory>(OtherInventoryWidget))
		{
			TempOtherInventory->RemoveItemAction();
			TempOtherInventory->OnItemInfoRemoved();
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

void ACustomPlayerController::DisplayMap(const FInputActionValue& Value)
{
	MapWidget = CreateWidget<UMapWidget>(this, PlayerData->MapClass);

	PushNewMenu(MapWidget, false, [this]
	{
		MapWidget = nullptr;
	});
}

void ACustomPlayerController::GoBackToPrecedentMenu(const FInputActionValue& Value)
{
	PopLastMenu();
}

void ACustomPlayerController::SelectedHotbar(const FInputActionValue& Value)
{
	int Index = (int)Value.Get<float>();

	if (Index >= MyPlayer->GetHotbarInventoryComponent()->GetInventoryLimitSize())
		return;

	
	if (Index == -1)
		Index = 0;

	
	MyPlayer->GetHotbarInventoryComponent()->UseItemAt(Index);
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
	PopLastMenu();

	SetInputMode(FInputModeUIOnly());

	PlayerCameraManager->StartCameraFade(0, 1, 1, FColor::Black, true, true);

	GetWorldTimerManager().SetTimer(SwitchLevel, [this]
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), PlayerData->MainMenuLevel);
	}, 1, false);
}

void ACustomPlayerController::PlaceObject(const FInputActionValue& Value)
{
	if (CreatedBuilding)
	{
		if (CreatedBuilding->CheckValidPlacement())
		{
			CreatedBuilding->PlaceBuilding();
			CreatedBuildings.Add(CreatedBuilding);
			CreatedBuilding = nullptr;
		}
	}
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

		return;
	}
	
	FStackedMenu& NewTopMenu = MenuStack.Last();

	if (NewTopMenu.Widget)
	{
		NewTopMenu.Widget->SetVisibility(ESlateVisibility::Visible);
	}

	UpdateGamePauseState();
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
}

void ACustomPlayerController::AddPlayerUIToViewport()
{
	SetInputMode(FInputModeGameOnly());

	if (PlayerData->HotbarInventoryWidgetClass)
	{
		HotbarInventoryWidget = CreateWidget<UInventory>(this, PlayerData->HotbarInventoryWidgetClass);
		HotbarInventoryWidget->BindToInventory(MyPlayer->GetHotbarInventoryComponent());
		HotbarInventoryWidget->BindTargetInventory(MyPlayer->GetInventoryComponent());
		HotbarInventoryWidget->AddToViewport();
	}

	HUDWidget = CreateWidget<UPlayerHud>(this, PlayerData->PlayerHudClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
		BindUIEvents();
	}
}

void ACustomPlayerController::ChangeArmorUI(UArmorDataAsset* NewArmor)
{
	if (UHotBar* HotBar = Cast<UHotBar>(HotbarInventoryWidget))
	{
		if (NewArmor->Type.MatchesTag(GT_Item_Armor_Helmet))
			HotBar->SetHelmetBrush(NewArmor->ItemIcon);
		else if  (NewArmor->Type.MatchesTag(GT_Item_Armor_Chest))
			HotBar->SetChestBrush(NewArmor->ItemIcon);
		else if  (NewArmor->Type.MatchesTag(GT_Item_Armor_Pant))
			HotBar->SetPantBrush(NewArmor->ItemIcon);
		else if  (NewArmor->Type.MatchesTag(GT_Item_Armor_Glove))
			HotBar->SetGloveBrush(NewArmor->ItemIcon);
	}
}

void ACustomPlayerController::CreateBuilding(TSubclassOf<ABuilding> BuildingClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CreatedBuilding = GetWorld()->SpawnActor<ABuilding>(
		BuildingClass,
		PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetCameraRotation().Vector() * ObjectPlacementRange),
		FRotator::ZeroRotator,
		SpawnParams
	);

	ObjectPlacementQueryParams.AddIgnoredActor(CreatedBuilding);
}
