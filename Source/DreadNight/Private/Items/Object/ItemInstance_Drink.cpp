#include "Items/Object/ItemInstance_Drink.h"

FName UItemInstance_Drink::GetActionName()
{
	return FName(TEXT("Drink"));
}

void UItemInstance_Drink::Use(AActor* Player)
{
}
