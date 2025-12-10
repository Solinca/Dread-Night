#include "Components/ManaComponent.h"

UManaComponent::UManaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CanRegen)
		RegenMana(DeltaTime);
}

void UManaComponent::AddMaxMana(float amount)
{
	MaxMana += amount;
	AddMana(amount);
}

void UManaComponent::AddMana(float amount)
{
	CurrentMana += amount;
	CurrentMana = FMath::Clamp(CurrentMana, 0.f, MaxMana);
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UManaComponent::RemoveMaxMana(float amount)
{
	MaxMana -= amount;
	CurrentMana = FMath::Clamp(CurrentMana, 0.f, MaxMana);
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UManaComponent::RemoveMana(float amount)
{
	CurrentMana -= amount;
	CurrentMana = FMath::Clamp(CurrentMana, 0.f, MaxMana);
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

float UManaComponent::GetManaRatio()
{
	return CurrentMana / MaxMana;
}

void UManaComponent::RegenMana(float DeltaTime)
{
	if (CurrentMana < MaxMana)
		CurrentMana += RegenSpeed * DeltaTime;
	CurrentMana = FMath::Clamp(CurrentMana, 0.f, MaxMana);
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UManaComponent::SetCanRegen(bool value)
{
	CanRegen = value;
}

float UManaComponent::GetRegenCoolDown()
{
	return RegenCooldown;
}

