#include "Components/ConditionStateComponent.h"

UConditionStateComponent::UConditionStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConditionStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RemoveHungerValue(DecreaseHungerRate * DeltaTime);

}

void UConditionStateComponent::AddHungerValue(float amount)
{
	HungerValue += amount;
	HungerValue = FMath::Clamp(HungerValue, 0.f, 100.f);

	if (HungerValue > 20.f)
	{
		States &= ~EConditionState::HUNGRY;
	}

	OnHungerChanged.Broadcast(GetHungerValueRatio(), EnumHasAnyFlags(States, EConditionState::HUNGRY));
}

void UConditionStateComponent::RemoveHungerValue(float amount)
{
	HungerValue -= amount;
	HungerValue = FMath::Clamp(HungerValue, 0.f, 100.f);

	if (HungerValue <= 20.f)
	{
		States |= EConditionState::HUNGRY;
	}

	OnHungerChanged.Broadcast(GetHungerValueRatio(), EnumHasAnyFlags(States, EConditionState::HUNGRY));
}

void UConditionStateComponent::ClearStates()
{
	HungerValue = 100.f;


	States = EConditionState::NONE;

	OnHungerChanged.Broadcast(GetHungerValueRatio(), EnumHasAnyFlags(States, EConditionState::HUNGRY));
}

float UConditionStateComponent::GetHungerValueRatio()
{
	return HungerValue / HungerMaxValue;
}

