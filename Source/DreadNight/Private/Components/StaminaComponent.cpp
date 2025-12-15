#include "Components/StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CanRegen)
		RegenStamina(DeltaTime);
}

void UStaminaComponent::AddMaxStamina(float amount)
{
	MaxStamina += amount;
	AddStamina(amount);
}

void UStaminaComponent::AddStamina(float amount)
{
	CurrentStamina += amount;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

void UStaminaComponent::RemoveMaxStamina(float amount)
{
	MaxStamina -= amount;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

/// <summary>
/// Remove a fixed amount of stamina
/// </summary>
void UStaminaComponent::RemoveStamina(float amount)
{
	CurrentStamina -= amount;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

float UStaminaComponent::GetStaminaRatio()
{
	return CurrentStamina / MaxStamina;
}

void UStaminaComponent::RegenStamina(float DeltaTime)
{
	if (CurrentStamina < MaxStamina)
		CurrentStamina += RegenSpeed * DeltaTime;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

void UStaminaComponent::SetCanRegen(bool value)
{
	CanRegen = value;
}

float UStaminaComponent::GetRegenCoolDown()
{
	return RegenCooldown;
}

float UStaminaComponent::GetCurrentStamina()
{
	return CurrentStamina;
}

