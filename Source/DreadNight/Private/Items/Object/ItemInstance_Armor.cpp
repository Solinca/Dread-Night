#include "Items/Object/ItemInstance_Armor.h"

#include "Items/Data/ArmorDataAsset.h"

FName UItemInstance_Armor::GetActionName()
{
	return FName(TEXT("Equip"));
}

 

UArmorDataAsset* UItemInstance_Armor::GetDataAsset()
{
	return ArmorDataAsset;
}

void UItemInstance_Armor::OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
	if (ArmorDataAsset = Cast<UArmorDataAsset>(DataAsset); !ArmorDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("DataAsset %s is not the expected type !"), *DataAsset->GetName());
	}
}
