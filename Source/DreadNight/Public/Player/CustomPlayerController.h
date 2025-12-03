// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <InputActionValue.h>
#include <EnhancedInputComponent.h>
#include "PlayerCharacter.h"
#include "CustomPlayerController.generated.h"



USTRUCT(BlueprintType)
struct FInputActionSetup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TObjectPtr<class UInputAction> Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	ETriggerEvent Event;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	FName Name;
};
/**
 * 
 */
UCLASS()
class DREADNIGHT_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	ACustomPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Values")
	TObjectPtr<APlayerCharacter> MyPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inputs")
	float CameraSensibility = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TObjectPtr<class UInputMappingContext> MappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	TArray<FInputActionSetup> IA_Setup;

private:

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void MoveEnd(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void LookEnd(const FInputActionValue& Value);

	UFUNCTION()
	void Jump(const FInputActionValue& Value);

	UFUNCTION()
	void Sprint(const FInputActionValue& Value);

	UFUNCTION()
	void SprintEnd(const FInputActionValue& Value);

	UFUNCTION()
	void Crouch(const FInputActionValue& Value);

	UFUNCTION()
	void CrouchEnd(const FInputActionValue& Value);

	UFUNCTION()
	void Aim(const FInputActionValue& Value);

	UFUNCTION()
	void AimEnd(const FInputActionValue& Value);

	UFUNCTION()
	void Attack(const FInputActionValue& Value);

	UFUNCTION()
	void AttackEnd(const FInputActionValue& Value);

	UFUNCTION()
	void Interact(const FInputActionValue& Value);

	UFUNCTION()
	void DisplayInventory(const FInputActionValue& Value);

	UFUNCTION()
	void DisplayGlossary(const FInputActionValue& Value);

	UFUNCTION()
	void DisplayMenu(const FInputActionValue& Value);

	UFUNCTION()
	void GoBackToPrecedentMenu(const FInputActionValue& Value);

	UFUNCTION()
	void SelectedHotbar(const FInputActionValue& Value);
};
