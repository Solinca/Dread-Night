#include "Components/HealthComponent.h"
#include "Player/PlayerCharacter.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::SetMaxHealth(float amount)
{
	MaxHealth = CurrentHealth = amount;
}

void UHealthComponent::AddMaxHealth(float amount, bool doesModifyCurrentHealth)
{
	MaxHealth += amount;

	if (doesModifyCurrentHealth)
	{
		AddHealth(amount);
	}
}

void UHealthComponent::AddHealth(float amount)
{
	CurrentHealth += amount;

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::RemoveMaxHealth(float amount, bool doesModifyCurrentHealth)
{
	MaxHealth -= amount;

	if (doesModifyCurrentHealth)
	{
		RemoveHealth(amount);
	}
}

void UHealthComponent::RemoveHealth(float amount)
{
	CurrentHealth -= amount;

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

float UHealthComponent::GetHealthRatio()
{
	return CurrentHealth / MaxHealth;
}


