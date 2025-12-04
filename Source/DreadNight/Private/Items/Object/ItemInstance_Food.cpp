#include "Items/Object/ItemInstance_Food.h"

FName UItemInstance_Food::GetActionName()
{
	return FName(TEXT("Eat"));
}

void UItemInstance_Food::Use(AActor* Player)
{
}
