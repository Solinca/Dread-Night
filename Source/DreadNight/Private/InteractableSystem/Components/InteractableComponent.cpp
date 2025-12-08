#include "InteractableSystem/Components/InteractableComponent.h"


void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	OutlineMaterialInstance = UMaterialInstanceDynamic::Create(OutlineMaterial, this);
}

UMaterialInstanceDynamic* UInteractableComponent::GetOutlineMaterial()
{
	return OutlineMaterialInstance;
}

const FText& UInteractableComponent::GetInteractionPromptText() const
{
	return InteractionText;
}