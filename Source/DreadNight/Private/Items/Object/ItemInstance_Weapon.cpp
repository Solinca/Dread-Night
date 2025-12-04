#include "Items/Object/ItemInstance_Weapon.h"

FName UItemInstance_Weapon::GetActionName()
{
	return FName(TEXT("Equip"));
}

void UItemInstance_Weapon::Use(AActor* Player)
{
}
