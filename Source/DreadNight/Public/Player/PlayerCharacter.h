// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class DREADNIGHT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	APlayerCharacter();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera = nullptr;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;


	UPROPERTY(BlueprintReadWrite)
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSprinting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Values")
	float CurrentCapsuleHH = 88.f;

};
