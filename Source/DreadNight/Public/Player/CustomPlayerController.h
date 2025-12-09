// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "CustomPlayerController.generated.h"


class UPauseMenu;

USTRUCT(BlueprintType)
struct FInputActionSetup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TObjectPtr<class UInputAction> Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	ETriggerEvent Event;

	UPROPERTY(EditAnywhere, meta = (FunctionReference, PrototypeFunction = "/Script/DreadNight.CustomPlayerController.Prototype_InputAction"))
	FMemberReference ActionName;


};
/**
 * 
 */
UCLASS()
class DREADNIGHT_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float CameraSensitivity = 75.f;

	/// <summary>
	/// Value Min and Max to restrain the pitch camera angle
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FVector2D ViewPitch = FVector2D(-55, 55);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TObjectPtr<class UInputMappingContext> MappingContextBase = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TObjectPtr<class UInputMappingContext> MappingContextMenu = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TArray<FInputActionSetup> IA_Setup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TArray<FInputActionSetup> IA_SetupMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float BaseMoveSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float SprintMoveSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float CrouchMoveSpeed = 300.f;

	//=========//
	//==Stats==//
	//=========//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
	float JumpStaminaCost = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
	float AttackStaminaCost = 20.f;//REPLACE WITH WEAPON STAMINA COST LATER

	/// <summary>
	/// Cost in amount/s
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
	float SprintStaminaCost = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mana")
	float SpellManaCost = 20.f;//REPLACE WITH WEAPON MANA COST LATER

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hunger")
	float HungerSprintCost = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hunger")
	float HungerJumpCost = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hunger")
	float HungerAttackCost = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sanity")
	float SanityOnDamageCost = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sanity")
	float SanityOnDarknessCost = 1.f;

	//==================//

	
	//=========//
	//==Widget==//
	//=========//

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPauseMenu> PauseMenuClass;

	UPROPERTY(Transient)
	TObjectPtr<UPauseMenu> PauseMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Logic")
	TSoftObjectPtr<UWorld> WorldMenu;
	
	//==================//
private:
	TQueue<TPair<UUserWidget*,TFunction<void()>>> MenuList; 
	
	TObjectPtr<APlayerCharacter> MyPlayer = nullptr;

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

	UFUNCTION(BlueprintCallable)
	void Aim(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Attack(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Interact(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void DisplayInventory(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void DisplayGlossary(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void DisplayMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void GoBackToPrecedentMenu(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void SelectedHotbar(const FInputActionValue& Value);
	
	void SaveGame();

	UFUNCTION()
	void GoBackToMenu();

	//Function to add a Menu to the menu list, so we can leave it with escape
	template<typename T>
	requires std::is_base_of_v<UUserWidget, T>
	void PushNewMenu(TObjectPtr<T>& Widget, TFunction<void()>&& OnCloseAction = []{})
	{
		if (MenuList.IsEmpty() && GetLocalPlayer())
		{
			if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (!MappingContextMenu)
				{
					return;
				}
				InputSystem->ClearAllMappings();
				InputSystem->AddMappingContext(MappingContextMenu, 0);
				SetInputMode(FInputModeGameAndUI());
			}
			SetShowMouseCursor(true);
		}
	
		Widget->AddToViewport();	 
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
			Widget = nullptr;
		};

		MenuList.Enqueue({Widget, MoveTemp(SafeCloseWidgetAction)});
	}

	//Call this function when you need to delete the last menu who has been push in the list
	UFUNCTION()
	void PopLastMenu();

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void PauseGame();

	UFUNCTION()
	void LeaveGame();
};
