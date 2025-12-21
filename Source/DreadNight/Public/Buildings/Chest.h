#pragma once

#include "CoreMinimal.h"
#include "Actors/Building.h"
#include "InteractableSystem/Interfaces/Interactable.h"
#include "InventorySystem/InventoryComponent.h"
#include "Chest.generated.h"

UCLASS()
class DREADNIGHT_API AChest : public AActor, public IInteractable
{
	GENERATED_BODY()

protected:
	AChest();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
};
