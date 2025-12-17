#include "Actors/BuildingStation.h"
#include "InteractableSystem/Components/InteractableComponent.h"

ABuildingStation::ABuildingStation()
{
	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Comp"));
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(TEXT("Crafting Component"));
}

void ABuildingStation::OnInteract_Implementation(AActor* Caller)
{
	
}

void ABuildingStation::OnFocusGained_Implementation()
{
	MeshComp->SetOverlayMaterial(InteractableComp->GetOutlineMaterial());
}

void ABuildingStation::OnFocusLost_Implementation()
{
	MeshComp->SetOverlayMaterial(nullptr);
}

FText ABuildingStation::GetInteractionPromptText_Implementation()
{
	return InteractableComp->GetInteractionPromptText();
}