#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance.h"
#include "Items/Data/ItemDataAsset.h"
#include "Items/Data/ItemGameplayTag.h"
#include "Items/Object/ItemInstance_Armor.h"
#include "Items/Object/ItemInstance_Drink.h"
#include "Items/Object/ItemInstance_Food.h"
#include "Items/Object/ItemInstance_Weapon.h"


UItemInstance* FItemInstanceFactory::CreateItem(UObject* Outer,UItemDataAsset* ItemDataAsset, const int StartStackSize)
{ 
	if (!ItemDataAsset)
		return nullptr;

	UItemInstance* NewItem = nullptr;
	if (ItemDataAsset->Type.MatchesTag(GT_Item_Weapon))
		NewItem = NewObject<UItemInstance_Weapon>(Outer);
	else if (ItemDataAsset->Type.MatchesTag(GT_Item_Armor))
		NewItem = NewObject<UItemInstance_Armor>(Outer);
	else if (ItemDataAsset->Type.MatchesTag(GT_Item_Food))
		NewItem = NewObject<UItemInstance_Food>(Outer);
	else if (ItemDataAsset->Type.MatchesTag(GT_Item_Drink))
		NewItem = NewObject<UItemInstance_Drink>(Outer);

	
	else if (ItemDataAsset->Type.MatchesTag(GT_Item))
		NewItem = NewObject<UItemInstance>(Outer);
	
	if (!NewItem)
		return nullptr;

	NewItem->SetupItemInstance(ItemDataAsset,StartStackSize);
	return NewItem;
}
