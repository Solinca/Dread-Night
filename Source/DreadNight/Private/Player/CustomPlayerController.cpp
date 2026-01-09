#include "Player/CustomPlayerController.h"
#include <string>
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
#include "Items/Object/ItemInstance_Building.h"
#include "Items/Object/ItemInstance_Weapon.h"
#include "Subsystems/World/WaveWorldSubsystem.h"
#include "Subsystems/World/DayCycleSubSystem.h"

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
	
	MyPlayer->GetArmorComponent()->OnArmorChanged.AddDynamic(this, &ThisClass::ChangeArmorUI);

	MyPlayer->GetInventoryComponent()->OnItemAddedToInventory.AddDynamic(this, &ThisClass::AddItemNotificationToViewport);
	
	MyPlayer->GetHotbarInventoryComponent()->OnItemAddedToInventory.AddDynamic(this, &ThisClass::AddItemNotificationToViewport);
	
	PlayerCameraManager->ViewPitchMin = PlayerData->ViewPitch.X;

	PlayerCameraManager->ViewPitchMax = PlayerData->ViewPitch.Y;

	ObjectPlacementQueryParams.bTraceComplex = true;
	ObjectPlacementQueryParams.AddIgnoredActor(GetPawn());

	MySettings = Cast<UMyGameUserSettings>(GEngine->GetGameUserSettings());
	
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	MyGameInstance->OnPCGEndGeneration.AddDynamic(this, &ThisClass::AddPlayerUIToViewport);
 
	if (UWaveWorldSubsystem* WaveWorldSubsystem = GetWorld()->GetSubsystem<UWaveWorldSubsystem>())
	{
		WaveWorldSubsystem->OnLastWaveEnd.AddDynamic(this, &ThisClass::TravelToVictoryLevel);
	}
	
	MyGameInstance->OnControllerEndBeginPlay.Broadcast();
	MyGameInstance->OnControllerEndBeginPlay.Clear();
	
	GetWorld()->GetSubsystem<UDayCycleSubSystem>()->OnDawnStart.AddDynamic(this, &ThisClass::SaveGame);
	
	OnePass = false;
}

void ACustomPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SaveIconHandle);
}

void ACustomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!OnePass)
	{
		ChangeSelectedSlotToFirstAvailable();
		OnePass = true;
	}
	
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

void ACustomPlayerController::OnDestroy()
{
	if (GetWorldTimerManager().IsTimerActive(SwitchLevel))
	{
		GetWorldTimerManager().ClearTimer(SwitchLevel);
	}
	
	if (GetWorldTimerManager().IsTimerActive(SaveIconHandle))
	{
		GetWorldTimerManager().ClearTimer(SaveIconHandle);
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
				PlayerData->TimeBeforeStartRegenStamina, false
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

		if (StaminaComponent->GetCurrentStamina() > 0.f && MyPlayer->GetCharacterMovement()->Velocity.Length() > 0.f)
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
			PlayerData->TimeBeforeStartRegenStamina, false
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
	if (!BuildingPreview) 
		return;
	
	FHitResult CameraHit;
	FHitResult GroundHit;
	FVector TargetLocation;
		
	if (GetWorld()->LineTraceSingleByChannel(
		CameraHit,
		PlayerCameraManager->GetCameraLocation(),
		PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetCameraRotation().Vector() * ObjectPlacementRange),
		ECC_WorldStatic,
		ObjectPlacementQueryParams) &&
		!CameraHit.GetActor()->IsA(ACharacter::StaticClass()))
	{
		TargetLocation = CameraHit.ImpactPoint;
	}
	else
	{
		TargetLocation = PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetCameraRotation().Vector() * ObjectPlacementRange);
	}
		
	FBox Box = BuildingPreview->GetMeshComponent()->CalcBounds(BuildingPreview->GetMeshComponent()->GetComponentTransform()).GetBox();

	if (GetWorld()->LineTraceSingleByChannel(
		GroundHit,
		TargetLocation + FVector(0.f, 0.f, BuildingPreview->GetActorLocation().Z - Box.Min.Z),
		TargetLocation - FVector(0.f, 0.f, 1000.f),
		ECC_WorldStatic,
		ObjectPlacementQueryParams))
	{
		BuildingPreview->SetActorLocation(GroundHit.ImpactPoint + FVector(0.f, 0.f, BuildingPreview->GetDataAsset()->DistanceFromTheGround));
	}
	else
	{
		BuildingPreview->SetActorLocation(TargetLocation);
	}
	
	BuildingPreview->CheckValidPlacement();
}

void ACustomPlayerController::ItemSpecialActionStart(const FInputActionValue& Value)
{
	UItemInstance* Item = MyPlayer->GetHotbarInventoryComponent()->GetItemAtSlot(MyPlayer->CurrentHotbarIndex);

	UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();

	if (Item && Item->GetDataAsset()->Type.GetTagName().ToString().Contains("Item.Weapon.Bow") && (MyPlayer->GetInventoryComponent()->Contains("Item.Weapon.Arrow", 1) || 
		MyPlayer->GetHotbarInventoryComponent()->Contains("Item.Weapon.Arrow", 1)))
	{
		MyPlayer->GetBowCombatComponent()->SetAiming(true);

		StaminaComponent->SetCanRegen(false);

		GetWorldTimerManager().ClearTimer(StaminaComponent->CoolDownTimer);
	}
}

void ACustomPlayerController::ItemSpecialActionStop(const FInputActionValue& Value)
{
	MyPlayer->GetBowCombatComponent()->SetAiming(false);

	UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();

	StaminaComponent->SetCanRegen(true);
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
	
	InventoryWidget->SetAnchorsInViewport(FAnchors(0.5f, 0.5f));
	InventoryWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
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

	if (Index == -1)
	{
		Index = 0;
	}

	if (MyPlayer->CurrentHotbarIndex == Index || Index >= MyPlayer->GetHotbarInventoryComponent()->GetInventoryLimitSize() ||MyPlayer->GetHotbarInventoryComponent()->IsSlotEmpty(Index))
	{
		return;
	}

	RemoveCurrentlyHoldItem();
	
	MyPlayer->CurrentHotbarIndex = Index;

	MyPlayer->GetHotbarInventoryComponent()->OnSelectedHotbarChanged.Broadcast(MyPlayer->CurrentHotbarIndex);

	ProcessHotbarSlot();
}

void ACustomPlayerController::ScrollHotbar(const FInputActionValue& Value)
{
	RemoveCurrentlyHoldItem();
	
	int InventoryLimit = MyPlayer->GetHotbarInventoryComponent()->GetInventoryLimitSize();

	bool validSpot = false;
	int iteration = 0;
	while (!validSpot && iteration < InventoryLimit)
	{
		MyPlayer->CurrentHotbarIndex = (MyPlayer->CurrentHotbarIndex + (int)Value.Get<float>() + InventoryLimit) % InventoryLimit;
		
		if (!MyPlayer->GetHotbarInventoryComponent()->IsSlotEmpty(MyPlayer->CurrentHotbarIndex))
		{
			validSpot = true;
		}
		iteration++;
	}
	if (!validSpot)
		MyPlayer->CurrentHotbarIndex = 0;
	
	MyPlayer->GetHotbarInventoryComponent()->OnSelectedHotbarChanged.Broadcast(MyPlayer->CurrentHotbarIndex);
	
	ProcessHotbarSlot();
}

void ACustomPlayerController::RemoveCurrentlyHoldItem()
{
	StopBuildingPlacement();

	MyPlayer->UnequipCurrentlyHeldItem();
}

void ACustomPlayerController::OnHotbarItemChanged(int Index)
{
	if (MyPlayer->CurrentHotbarIndex == Index)
	{
		RemoveCurrentlyHoldItem();

		ProcessHotbarSlot();
	}
	if (MyPlayer->GetHotbarInventoryComponent()->IsSlotEmpty(MyPlayer->CurrentHotbarIndex))
	{
		ChangeSelectedSlotToFirstAvailable();
	}
}

void ACustomPlayerController::UseItem(const FInputActionValue& Value)
{
	UItemInstance* Item = MyPlayer->GetHotbarInventoryComponent()->GetItemAtSlot(MyPlayer->CurrentHotbarIndex);
	
	if (BuildingPreview && BuildingPreview->CheckValidPlacement())
	{
		BuildingPreview->PlaceBuilding();

		UItemInstance_Building* BuildingItem = Cast<UItemInstance_Building>(Item);
		if (Item && BuildingItem && BuildingItem->GetStackNumber() > 0)
		{
			MyPlayer->GetHotbarInventoryComponent()->RemoveItemsAt(MyPlayer->CurrentHotbarIndex, 1);
			
			if (MyPlayer->GetHotbarInventoryComponent()->IsSlotEmpty(MyPlayer->CurrentHotbarIndex))
			{
				ChangeSelectedSlotToFirstAvailable();
			}

		}
		
		StopBuildingPlacement();
		OnHotbarItemChanged(MyPlayer->CurrentHotbarIndex);
		return;
	}
	
	if (!Item)
	{
		return;
	}
	
	if (Item->GetDataAsset()->Type.GetTagName().ToString().Contains("Item.Food"))
	{
		MyPlayer->GetHotbarInventoryComponent()->UseItemAt(MyPlayer->CurrentHotbarIndex);
		
		if (MyPlayer->GetHotbarInventoryComponent()->IsSlotEmpty(MyPlayer->CurrentHotbarIndex))
		{
			ChangeSelectedSlotToFirstAvailable();
		}

		return;
	}

	USwordCombatComponent* SwordCombatComponent = MyPlayer->GetSwordCombatComponent();
	
	UStaminaComponent* StaminaComponent = MyPlayer->GetStaminaComponent();
	
	UBowCombatComponent* BowCombatComponent = MyPlayer->GetBowCombatComponent();

	UInventoryComponent* InventoryComponent = MyPlayer->GetInventoryComponent();
	
	UInventoryComponent* HotBarInventoryComponent = MyPlayer->GetHotbarInventoryComponent();

	if (Item->GetDataAsset()->Type.GetTagName().ToString().Contains("Item.Weapon.Sword"))
	{
		if (!SwordCombatComponent->GetIsAttacking() && StaminaComponent->GetCurrentStamina() > 0.f)
		{
			SwordCombatComponent->Attack();

			StaminaComponent->SetCanRegen(false);

			GetWorldTimerManager().ClearTimer(StaminaComponent->CoolDownTimer);

			StaminaComponent->RemoveStamina(PlayerData->AttackStaminaCost);

			MyPlayer->GetConditionStateComponent()->RemoveHungerValue(PlayerData->HungerAttackCost);
		}
	}
	else if (Item->GetDataAsset()->Type.GetTagName().ToString().Contains("Item.Weapon.Bow"))
	{
		if (BowCombatComponent->IsAiming() && BowCombatComponent->CanShoot() && StaminaComponent->GetCurrentStamina() > 0.f)
		{
			BowCombatComponent->Shoot();

			if (!HotBarInventoryComponent->RemoveItemsByTag("Item.Weapon.Arrow", 1))
			{
				InventoryComponent->RemoveItemsByTag("Item.Weapon.Arrow", 1);
			}

			MyPlayer->GetConditionStateComponent()->RemoveHungerValue(PlayerData->HungerAttackCost);
		}
	}
}

void ACustomPlayerController::SaveGame()
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->Save(GetWorld());

		if (SaveWidget)
			return;
		
		SaveWidget = CreateWidget<UUserWidget>(this, PlayerData->SaveWidgetClass);
		SaveWidget->AddToViewport();
		GetWorldTimerManager().SetTimer(SaveIconHandle, [this]
		{ 
			SaveWidget->RemoveFromParent();
		}, PlayerData->SaveIconDuration, false);
	}
}

void ACustomPlayerController::LoadGame()
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->Load(GetWorld());
	}
}

void ACustomPlayerController::DeleteSave()
{
	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->DeleteSave();
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
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}

void ACustomPlayerController::ShowGameOver()
{
	TObjectPtr<UUserWidget> WidgetGameOver = CreateWidget<UUserWidget>(this, PlayerData->GameOverClass);

	DeleteSave();
	
	PushNewMenu(WidgetGameOver, true, []() {}, false);

	UGameplayStatics::PlaySound2D(this, PlayerData->GameOverSound);
	
	OnDestroy();
}

void ACustomPlayerController::TravelToVictoryLevel()
{
	FEndVictoryFunctor Functor = {.Controller = this, .World = GetWorld()};

	Functor();
}

void ACustomPlayerController::BindUIEvents()
{
	MyPlayer->GetHealthComponent()->OnHealthChanged.AddDynamic(HUDWidget, &UPlayerHud::UpdateHealthBar);
	MyPlayer->GetStaminaComponent()->OnStaminaChanged.AddDynamic(HUDWidget, &UPlayerHud::UpdateStaminaBar);
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
		MyPlayer->GetHotbarInventoryComponent()->OnHotbarItemChanged.AddDynamic(this, &ACustomPlayerController::OnHotbarItemChanged);
	}

	HUDWidget = CreateWidget<UPlayerHud>(this, PlayerData->PlayerHudClass);

	if (HUDWidget)
	{
		HUDWidget->AddToViewport();

		BindUIEvents();
	}

	UUserWidget* PlayerDamageWidget = CreateWidget<UUserWidget>(this, PlayerData->PlayerDamageUI);

	if (PlayerDamageWidget)
	{
		PlayerDamageWidget->AddToViewport();
	}
}

void ACustomPlayerController::ChangeArmorUI(UArmorDataAsset* ArmorData, bool IsEquipped)
{
	if (UHotBar* HotBar = Cast<UHotBar>(HotbarInventoryWidget))
	{
		if (ArmorData->Type.MatchesTag(GT_Item_Armor_Helmet))
		{
			HotBar->SetHelmetBrush(IsEquipped ? ArmorData->ItemIcon : nullptr);
		}
		else if (ArmorData->Type.MatchesTag(GT_Item_Armor_Chest))
		{
			HotBar->SetChestBrush(IsEquipped ? ArmorData->ItemIcon : nullptr);
		}
		else if (ArmorData->Type.MatchesTag(GT_Item_Armor_Pant))
		{
			HotBar->SetPantBrush(IsEquipped ? ArmorData->ItemIcon : nullptr);
		}
		else if (ArmorData->Type.MatchesTag(GT_Item_Armor_Shoes))
		{
			HotBar->SetShoesBrush(IsEquipped ? ArmorData->ItemIcon : nullptr);
		}
	}
}

void ACustomPlayerController::CreateBuilding(UBuildingDataAsset* BuildingData)
{
	if (BuildingPreview)
	{
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BuildingPreview = GetWorld()->SpawnActor<ABuilding>(
		BuildingPreviewClass,
		PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetCameraRotation().Vector() * ObjectPlacementRange),
		FRotator::ZeroRotator,
		SpawnParams
	);

	BuildingPreview->SetDataAsset(BuildingData);

	ObjectPlacementQueryParams.AddIgnoredActor(BuildingPreview);
}

void ACustomPlayerController::StopBuildingPlacement()
{
	if (BuildingPreview)
	{
		BuildingPreview->Destroy();

		BuildingPreview = nullptr;
	}
}


void ACustomPlayerController::CloseInventoryAction(const FInputActionValue& Value)
{
    if (InventoryWidget)
    {
        InventoryWidget->RemoveItemAction();
    }
}

void ACustomPlayerController::ProcessHotbarSlot()
{
	if (MyPlayer)
	{
		MyPlayer->ProcessHotbarSlot();
	}
}

void ACustomPlayerController::ChangeSelectedSlotToFirstAvailable()
{
	RemoveCurrentlyHoldItem();
			
	MyPlayer->CurrentHotbarIndex=0;
	for (int i = 0; i <  MyPlayer->GetHotbarInventoryComponent()->GetInventoryLimitSize(); i++)
	{
		if (!MyPlayer->GetHotbarInventoryComponent()->IsSlotEmpty(MyPlayer->CurrentHotbarIndex))
		{
			break;
		}
		MyPlayer->CurrentHotbarIndex++;
	}
			
	if (MyPlayer->CurrentHotbarIndex >=  MyPlayer->GetHotbarInventoryComponent()->GetInventoryLimitSize())
	{
		MyPlayer->CurrentHotbarIndex = 0;
	}
	MyPlayer->GetHotbarInventoryComponent()->OnSelectedHotbarChanged.Broadcast(MyPlayer->CurrentHotbarIndex);
	
	ProcessHotbarSlot();
}

void ACustomPlayerController::AddItemNotificationToViewport(UItemInstance* Data, int32 Quantity)
{
	HUDWidget->AddItemNotification(Data, Quantity, PlayerData->ItemNotificationClass);
}
