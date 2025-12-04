#include "Items/Object/ItemInstance_Armor.h"

FName UItemInstance_Armor::GetActionName()
{
	return FName(TEXT("Equip"));
}

void UItemInstance_Armor::Use(AActor* Player)
{
}
