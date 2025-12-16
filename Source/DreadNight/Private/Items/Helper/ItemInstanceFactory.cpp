#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance.h"
#include "Items/Data/ItemDataAsset.h"
#include "Items/Data/ItemGameplayTag.h"
#include "Items/Object/ItemInstance_Armor.h"
#include "Items/Object/ItemInstance_ChestArmor.h"
#include "Items/Object/ItemInstance_Food.h"
#include "Items/Object/ItemInstance_HelmetArmor.h"
#include "Items/Object/ItemInstance_Weapon.h"


UItemInstance* FItemInstanceFactory::CreateItem(UObject* Outer,UItemDataAsset* ItemDataAsset, const int StartStackSize)
{ 
	if (!ItemDataAsset)
		return nullptr;

	UItemInstance* NewItem = nullptr;
	if (ItemDataAsset->Type.MatchesTag(GT_Item_Weapon))
		NewItem = NewObject<UItemInstance_Weapon>(Outer);
	else if (ItemDataAsset->Type.MatchesTag(GT_Item_Armor_Chest))
		NewItem = NewObject<UItemInstance_ChestArmor>(Outer);
	else if (ItemDataAsset->Type.MatchesTag(GT_Item_Armor_Helmet))
		NewItem = NewObject<UItemInstance_HelmetArmor>(Outer);
	else if (ItemDataAsset->Type.MatchesTag(GT_Item_Food))
		NewItem = NewObject<UItemInstance_Food>(Outer);

	
	else if (ItemDataAsset->Type.MatchesTag(GT_Item))
		NewItem = NewObject<UItemInstance>(Outer);
	
	if (!NewItem)
		return nullptr;

	NewItem->SetupItemInstance(ItemDataAsset,StartStackSize);
	return NewItem;
}
