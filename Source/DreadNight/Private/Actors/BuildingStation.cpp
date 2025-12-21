#include "Actors/BuildingStation.h"
#include "InteractableSystem/Components/InteractableComponent.h"

ABuildingStation::ABuildingStation()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Comp"));
	
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(TEXT("Crafting Component"));
}

void ABuildingStation::OnFocusGained_Implementation()
{
	Mesh->SetOverlayMaterial(InteractableComp->GetOutlineMaterial());
}

void ABuildingStation::OnFocusLost_Implementation()
{
	Mesh->SetOverlayMaterial(nullptr);
}

FText ABuildingStation::GetInteractionPromptText_Implementation()
{
	return InteractableComp->GetInteractionPromptText();
}