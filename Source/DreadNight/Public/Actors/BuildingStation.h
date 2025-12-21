#pragma once

#include "CoreMinimal.h"
#include "Actors/Building.h"
#include "InteractableSystem/Interfaces/Interactable.h"
#include "Crafting/Components/CraftingComponent.h"
#include "BuildingStation.generated.h"

class UInteractableComponent;

UCLASS()
class DREADNIGHT_API ABuildingStation : public AActor, public IInteractable
{
	GENERATED_BODY()
	
protected:
	ABuildingStation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInteractableComponent> InteractableComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCraftingComponent> CraftingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

public:
	virtual void OnFocusGained_Implementation() override;
	
	virtual void OnFocusLost_Implementation() override;
	
	virtual FText GetInteractionPromptText_Implementation() override;

	UCraftingComponent* GetCraftingComponent() const { return CraftingComponent; }
};