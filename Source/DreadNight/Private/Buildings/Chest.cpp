#include "Buildings/Chest.h"

AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = false;
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");
}

void AChest::BeginPlay()
{
	Super::BeginPlay();
}
