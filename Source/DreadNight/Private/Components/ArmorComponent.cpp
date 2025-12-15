#include "Components/ArmorComponent.h"

UArmorComponent::UArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UArmorComponent::SetArmorDmgReductionMultiplier(float NewMultiplier)
{
	CurrentArmorDmgReductionMultiplier = NewMultiplier;
}

void UArmorComponent::SetHelmetDmgReductionMultiplier(float NewMultiplier)
{
	CurrentHelmetDmgReductionMultiplier = NewMultiplier;
}
