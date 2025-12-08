// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class DREADNIGHT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSprinting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float CapsuleMaxHalfHeight = 88.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float CurrentCapsuleHalfHeight = CapsuleMaxHalfHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float CapsuleCrouchedHalfHeight = 44.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float LerpCrouchSpeed = 8.f;

public:

	UFUNCTION()
	bool GetIsCrouching();

	UFUNCTION()
	void SetIsCrouching(bool value);

	UFUNCTION()
	bool GetIsSprinting();

	UFUNCTION()
	void SetIsSprinting(bool value);

	UFUNCTION()
	float GetCurentCapsuleHalfHeight();

	UFUNCTION()
	void SetCurentCapsuleHalfHeight(float value);

	UFUNCTION()
	void UpdateCrouching(float deltatime);

};
