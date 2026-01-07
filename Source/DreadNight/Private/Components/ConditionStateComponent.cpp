#include "Components/ConditionStateComponent.h"

UConditionStateComponent::UConditionStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConditionStateComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHunger = PlayerData->MaxHunger;
}

void UConditionStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RemoveHungerValue(PlayerData->HungerDecreasePerSecond * DeltaTime);
}

void UConditionStateComponent::AddHungerValue(float amount)
{
	CurrentHunger += amount;

	CurrentHunger = FMath::Clamp(CurrentHunger, 0.f, 100.f);

	OnHungerChanged.Broadcast(GetHungerValueRatio());
}

void UConditionStateComponent::RemoveHungerValue(float amount)
{
	CurrentHunger -= amount;

	CurrentHunger = FMath::Clamp(CurrentHunger, 0.f, 100.f);

	OnHungerChanged.Broadcast(GetHungerValueRatio());
}

float UConditionStateComponent::GetHungerValueRatio()
{
	return CurrentHunger / PlayerData->MaxHunger;
}
