#pragma once

#include "CoreMinimal.h"
#include "Actors/Building.h"
#include "InteractableSystem/Interfaces/Interactable.h"
#include "InventorySystem/InventoryComponent.h"
#include "Chest.generated.h"

UCLASS()
class DREADNIGHT_API AChest : public ABuilding, public IInteractable
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;
	
	AChest();
	
	virtual void BeginPlay() override;
};
