// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "CustomPlayerController.generated.h"



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
	TObjectPtr<class UInputMappingContext> MappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TArray<FInputActionSetup> IA_Setup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float BaseMoveSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float SprintMoveSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float CrouchMoveSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMapWidget> MapWidgetClass;


private:

	TObjectPtr<APlayerCharacter> MyPlayer = nullptr;

	UPROPERTY()
	TObjectPtr<class UMapWidget> MapWidgetInstance = nullptr;

	bool bIsMapOpen = false;

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

	UFUNCTION(BlueprintCallable)
	void DisplayMap(const FInputActionValue& Value);
};
