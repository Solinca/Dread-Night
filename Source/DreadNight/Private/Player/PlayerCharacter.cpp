// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm Component");
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>("Stamina Component");
	ManaComponent = CreateDefaultSubobject<UManaComponent>("Mana Component");
	ConditionStateComponent = CreateDefaultSubobject<UConditionStateComponent>("ConditionState Component");
}

bool APlayerCharacter::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	HealthComponent->RemoveHealth(Damage);

	return true;
}

bool APlayerCharacter::GetIsCrouching()
{
	return bIsCrouching;
}

void APlayerCharacter::SetIsCrouching(bool value)
{
	bIsCrouching = value;
}

bool APlayerCharacter::GetIsSprinting()
{
	return bIsSprinting;
}

void APlayerCharacter::SetIsSprinting(bool value)
{
	bIsSprinting = value;
}

float APlayerCharacter::GetCurentCapsuleHalfHeight()
{
	return CurrentCapsuleHalfHeight;
}

void APlayerCharacter::SetCurentCapsuleHalfHeight(float value)
{
	CurrentCapsuleHalfHeight = value;
	GetCapsuleComponent()->SetCapsuleHalfHeight(value);
}

void APlayerCharacter::UpdateCrouching(float deltatime)
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		if (bIsCrouching)
			SetCurentCapsuleHalfHeight(FMath::FInterpTo(CurrentCapsuleHalfHeight, CapsuleCrouchedHalfHeight, deltatime, LerpCrouchSpeed));
		else
			SetCurentCapsuleHalfHeight(FMath::FInterpTo(CurrentCapsuleHalfHeight, CapsuleMaxHalfHeight, deltatime, LerpCrouchSpeed));
	}
}

UStaminaComponent* APlayerCharacter::GetStaminaComponent()
{
	return StaminaComponent;
}

UManaComponent* APlayerCharacter::GetManaComponent()
{
	return ManaComponent;
}

UHealthComponent* APlayerCharacter::GetHealthComponent()
{
	return HealthComponent;
}

UConditionStateComponent* APlayerCharacter::GetConditionStateComponent()
{
	return ConditionStateComponent;
}


