#pragma once

#include "CoreMinimal.h"
#include "Actors/Building.h"
#include "InteractableSystem/Interfaces/Interactable.h"
#include "Crafting/Components/CraftingComponent.h"
#include "BuildingStation.generated.h"

class UInteractableComponent;

UCLASS()
class DREADNIGHT_API ABuildingStation : public ABuilding, public IInteractable
{

	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInteractableComponent> InteractableComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCraftingComponent> CraftingComponent;

	ABuildingStation();

public:

	virtual void OnInteract_Implementation(AActor* Caller) override;
	virtual void OnFocusGained_Implementation() override;
	virtual void OnFocusLost_Implementation() override;
	virtual FText GetInteractionPromptText_Implementation() override;

	UCraftingComponent* GetCraftingComponent() const { return CraftingComponent; }

};