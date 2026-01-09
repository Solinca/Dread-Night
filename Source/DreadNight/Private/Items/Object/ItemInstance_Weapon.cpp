#include "Items/Object/ItemInstance_Weapon.h"
#include "Items/Data/WeaponDataAsset.h"
#include "Components/SwordCombatComponent.h"
#include "Components/BowCombatComponent.h"

FName UItemInstance_Weapon::GetActionName()
{
	return FName(TEXT("Equip"));
}

UWeaponDataAsset* UItemInstance_Weapon::GetDataAsset()
{
	return WeaponDataAsset;
}

void UItemInstance_Weapon::OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
	if (WeaponDataAsset = Cast<UWeaponDataAsset>(DataAsset); !WeaponDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("DataAsset %s is not the expected type !"), *DataAsset->GetName());
	}
}
