#include "Buildings/Chest.h"

AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");
}
