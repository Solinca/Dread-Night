#include "InteractableSystem/Components/InteractableComponent.h"


void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	OutlineMaterialInstance = UMaterialInstanceDynamic::Create(OutlineMaterial, this);
}

UMaterialInstanceDynamic* UInteractableComponent::GetOutlineMaterial(const bool bGainedFocus)
{
	return bGainedFocus ? OutlineMaterialInstance : nullptr;
}

const FText& UInteractableComponent::GetInteractionPromptText() const
{
	return InteractionText;
}