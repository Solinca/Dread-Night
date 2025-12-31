#include "Components/ArmorComponent.h"
#include "Items/Data/ArmorDataAsset.h"
#include "Items/Data/ItemGameplayTag.h"

UArmorComponent::UArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UArmorComponent::EquipArmor(UArmorDataAsset* Armor)
{	
	if (Armor->Type.MatchesTag(GT_Item_Armor_Helmet))
	{
		if (CurrentHelmet)
		{
			TotalDamageReduction -= CurrentHelmet->FlatDamageReduction;
		}

		CurrentHelmet = Armor;
	}
	else if (Armor->Type.MatchesTag(GT_Item_Armor_Chest))
	{
		if (CurrentChestPlate)
		{
			TotalDamageReduction -= CurrentChestPlate->FlatDamageReduction;
		}

		CurrentChestPlate = Armor;
	}
	else if (Armor->Type.MatchesTag(GT_Item_Armor_Pant))
	{
		if (CurrentPants)
		{
			TotalDamageReduction -= CurrentPants->FlatDamageReduction;
		}

		CurrentPants = Armor;
	}
	else if (Armor->Type.MatchesTag(GT_Item_Armor_Shoes))
	{
		if (CurrentBoots)
		{
			TotalDamageReduction -= CurrentBoots->FlatDamageReduction;
		}

		CurrentBoots = Armor;
	}

	TotalDamageReduction += Armor ? Armor->FlatDamageReduction : 0;
	
	OnArmorChanged.Broadcast(Armor, true);
}
