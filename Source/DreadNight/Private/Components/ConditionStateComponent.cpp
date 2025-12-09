#include "Components/ConditionStateComponent.h"

UConditionStateComponent::UConditionStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConditionStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RemoveHungerValue(DecreaseHungerRate * DeltaTime);
	RemoveThirstValue(DecreaseThirstRate * DeltaTime);

}

void UConditionStateComponent::AddHungerValue(float amount)
{
	HungerValue += amount;
	HungerValue = FMath::Clamp(HungerValue, 0.f, 100.f);
	OnHungerChanged.Broadcast(HungerValue, EnumHasAnyFlags(States, EConditionState::HUNGRY));
}

void UConditionStateComponent::RemoveHungerValue(float amount)
{
	HungerValue -= amount;
	HungerValue = FMath::Clamp(HungerValue, 0.f, 100.f);
	OnHungerChanged.Broadcast(HungerValue, EnumHasAnyFlags(States, EConditionState::HUNGRY));
}

void UConditionStateComponent::AddThirstValue(float amount)
{
	ThirstValue += amount;
	ThirstValue = FMath::Clamp(ThirstValue, 0.f, 100.f);
	OnThirstChanged.Broadcast(ThirstValue, EnumHasAnyFlags(States, EConditionState::THIRSTY));
}

void UConditionStateComponent::RemoveThirstValue(float amount)
{
	ThirstValue -= amount;
	ThirstValue = FMath::Clamp(ThirstValue, 0.f, 100.f);
	OnThirstChanged.Broadcast(ThirstValue, EnumHasAnyFlags(States, EConditionState::THIRSTY));
}

void UConditionStateComponent::AddSanityValue(float amount)
{
	SanityValue += amount;
	SanityValue = FMath::Clamp(SanityValue, 0.f, 100.f);
	OnSanityChanged.Broadcast(SanityValue, EnumHasAnyFlags(States, EConditionState::AFRAID));
}

void UConditionStateComponent::RemoveSanityValue(float amount)
{
	SanityValue -= amount;
	SanityValue = FMath::Clamp(SanityValue, 0.f, 100.f);
	OnSanityChanged.Broadcast(SanityValue, EnumHasAnyFlags(States, EConditionState::AFRAID));
}

void UConditionStateComponent::ClearStates()
{
	HungerValue = 100.f;
	ThirstValue = 100.f;
	SanityValue = 100.f;

	States = EConditionState::NONE;

	OnHungerChanged.Broadcast(HungerValue, EnumHasAnyFlags(States, EConditionState::HUNGRY));
	OnThirstChanged.Broadcast(ThirstValue, EnumHasAnyFlags(States, EConditionState::THIRSTY));
	OnSanityChanged.Broadcast(SanityValue, EnumHasAnyFlags(States, EConditionState::AFRAID));
}

float UConditionStateComponent::GetHungerValueRatio()
{
	return HungerValue / HungerMaxValue;
}

float UConditionStateComponent::GetThirstValueRatio()
{
	return ThirstValue / ThirstMaxValue;
}

float UConditionStateComponent::GetSanityValueRatio()
{
	return SanityValue / SanityMaxValue;
}

