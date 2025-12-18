#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "CustomPlayerController.generated.h"

class UPlayerHud;
class UPauseMenu;
class UOptionsWidget;
class UInventory;
class UGlossary;
class ABuilding;

USTRUCT(BlueprintType)
struct FInputActionSetup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TObjectPtr<UInputAction> Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	ETriggerEvent Event = ETriggerEvent::Triggered;

	UPROPERTY(EditAnywhere, meta = (FunctionReference, PrototypeFunction = "/Script/DreadNight.CustomPlayerController.Prototype_InputAction"))
	FMemberReference ActionName;
};

struct FStackedMenu
{
	TObjectPtr<UUserWidget> Widget;

	bool bTriggerPause;

	TFunction<void()> OnCloseAction;

	bool bCanBeQuit = true;
};

UCLASS()
class DREADNIGHT_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TObjectPtr<class UInputMappingContext> MappingContextBase = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TObjectPtr<class UInputMappingContext> MappingContextMenu = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TArray<FInputActionSetup> IA_Setup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TArray<FInputActionSetup> IA_SetupMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UPlayerDataAsset> PlayerData;

private:
	UPROPERTY(Transient)
	TObjectPtr<UPauseMenu> PauseMenuWidget;

	UPROPERTY(Transient)
	TObjectPtr<UInventory> InventoryWidget;
	
	UPROPERTY(Transient)
	TObjectPtr<UInventory> HotbarInventoryWidget;

	UPROPERTY(Transient)
	TObjectPtr<UInventory> OtherInventoryWidget;
	
	UPROPERTY(Transient)
	TObjectPtr<UOptionsWidget> OptionsWidget;
	
	UPROPERTY(Transient)
	TObjectPtr<UPlayerHud> HUDWidget;

	UPROPERTY(Transient)
	TObjectPtr<UGlossary> GlossaryWidget;

	UPROPERTY(Transient)
	TObjectPtr<UMapWidget> MapWidget;

	TArray<FStackedMenu> MenuStack;

	UPROPERTY(Transient)
	TObjectPtr<class UMyGameUserSettings> MySettings = nullptr;

	int32 PauseCounter = 0;

	void UpdateGamePauseState();
	
	UPROPERTY(Transient)
	TObjectPtr<APlayerCharacter> MyPlayer = nullptr;
	FCollisionQueryParams ObjectPlacementQueryParams;

#if WITH_EDITOR
	UFUNCTION(BlueprintInternalUseOnly)
	void Prototype_InputAction(const FInputActionValue& Value) {};
#endif

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Jump(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Sprint(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void SprintEnd(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Crouch(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void CrouchEnd(const FInputActionValue& Value);

	UFUNCTION()
	void UpdateCrouching(float deltatime);

	UFUNCTION()
	void UpdateObjectPlacement();

	UFUNCTION(BlueprintCallable)
	void Aim(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void StopAim(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Attack(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Interact(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void DisplayInventory(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void DisplayOtherInventory(UInventoryComponent* OtherInventory);
	
	UFUNCTION(BlueprintCallable)
	void DisplayGlossary(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void DisplayMenu(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void DisplayMap(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void GoBackToPrecedentMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void SelectedHotbar(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION()
	void GoBackToMenu();

	UFUNCTION(BlueprintCallable)
	void PlaceObject(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void RotateObject(const FInputActionValue& Value);

	UPROPERTY()
	TArray<AActor*> CreatedBuildings;
	
	UPROPERTY()
	ABuilding* CreatedBuilding = nullptr;

	UPROPERTY(EditAnywhere)
	float BuildingRotationSpeed;

	UPROPERTY(EditAnywhere)
	float ObjectPlacementRange = 200.f;

	// Function to add a Menu to the menu list, so we can leave it with escape
	template<typename T>
	requires std::is_base_of_v<UUserWidget, T>
	void PushNewMenu(TObjectPtr<T>& Widget, bool bPausesGame, TFunction<void()>&& OnCloseAction = []{}, bool bCanBeQuit = true)
	{
		if (MenuStack.IsEmpty() && GetLocalPlayer())
		{
			if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (MappingContextMenu)
				{
					InputSystem->ClearAllMappings();

					InputSystem->AddMappingContext(MappingContextMenu, 0);
				}
			}

			SetInputMode(FInputModeGameAndUI());

			SetShowMouseCursor(true);
		}
		else
		{
			FStackedMenu& TopMenu = MenuStack.Last();

			if (TopMenu.Widget)
			{
				TopMenu.Widget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	
		if (!Widget->IsInViewport())
		{
			Widget->AddToViewport();
		}

		Widget->SetVisibility(ESlateVisibility::Visible);
		
		auto SafeCloseWidgetAction = [&, CloseAction = MoveTemp(OnCloseAction), IsFirstPass = true]() mutable
		{
			if (!IsFirstPass)
			{
				return;
			}

			IsFirstPass = false; 

			if (CloseAction)
			{
				CloseAction();
			}
		};

		MenuStack.Push({ Widget, bPausesGame, MoveTemp(SafeCloseWidgetAction), bCanBeQuit });

		if (bPausesGame)
		{
			PauseCounter++;
		}
		
		UpdateGamePauseState();
	}

	// Call this function when you need to delete the last menu who has been push in the list
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void PopLastMenu();
	
	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void PauseGame();

	UFUNCTION()
	void AccessOptions();

	UFUNCTION()
	void QuitOptions();

	UFUNCTION()
	void LeaveGame();
	
	UFUNCTION()
	void ShowGameOver();

	void BindUIEvents();

	UFUNCTION()
	void AddPlayerUIToViewport();
	
	UFUNCTION()
	void ChangeArmorUI(UArmorDataAsset* NewArmor);
	
public:
	
	void CreateBuilding(TSubclassOf<ABuilding> BuildingClass);
};
