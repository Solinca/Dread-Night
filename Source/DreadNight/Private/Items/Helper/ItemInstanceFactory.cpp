// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Helper/ItemInstanceFactory.h"
#include "Items/Object/ItemInstance.h"
#include "Items/Data/ItemDataAsset.h"
#include "Items/Data/ItemGameplayTag.h"

 
UItemInstance* FItemInstanceFactory::CreateItem(UItemDataAsset* ItemDataAsset, const int StartStackSize)
{ 
	if (!ItemDataAsset)
		return nullptr;

	UItemInstance* NewItem = NewObject<UItemInstance>();
	if (!NewItem)
		return nullptr;

	NewItem->SetupItemInstance(ItemDataAsset,StartStackSize);
	return NewItem;
}
