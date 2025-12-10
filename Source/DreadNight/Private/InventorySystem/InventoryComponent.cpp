#include "InventorySystem/InventoryComponent.h"
#include "Items/Interface/UsableItem.h"
#include "Items/Data/ItemDataAsset.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Items.SetNum(Size);
}

void UInventoryComponent::AddItem(TObjectPtr<UItemInstance> Item)
{
	if (!Item || IsFull())
		return;
	
	if (Contains(Item->GetItemDataAsset(), 1))
	{
		for (UItemInstance* tempItem : Items)
		{
			if (tempItem->GetItemDataAsset() == Item->GetItemDataAsset())
			{
				if (tempItem->GetStackNumber() != tempItem->GetItemDataAsset()->StackLimit)
				{
					tempItem->TryStackWith(Item);
					
					if (Item->IsEmpty())
						return;
				}
			}
		}
	}
	
	if (Item->IsEmpty())
		return;
	
	Items[GetEmptySlot()] = Item;
}

void UInventoryComponent::RemoveItemsByType(TObjectPtr<UItemDataAsset> Item)
{
	if (!Item)
		return;
	
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i]->GetItemDataAsset() == Item)
		{
			Items[i]->TryUse(1);
			
			if (Items[i]->IsEmpty())
				Items[i] = nullptr;
			
			return;
		}
	}
}

void UInventoryComponent::RemoveItemsAt(int SlotIndex, int Amount)
{
	Items[SlotIndex]->TryUse(Amount);
	
	if (Items[SlotIndex]->IsEmpty())
		Items[SlotIndex] = nullptr;
}

void UInventoryComponent::UseItemByType(TObjectPtr<UItemDataAsset> Item)
{
	if (!Item)
		return;

	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i]->GetItemDataAsset() == Item)
		{
			if (TObjectPtr<IUsableItem> UsableItem = Cast<IUsableItem>(Items[i]))
			{
				UsableItem->Use(GetOwner());
				Items[i]->TryUse(1);
				
				if (Items[i]->IsEmpty())
					Items[i] = nullptr;
				
				return;
			}
		}
	}
}

void UInventoryComponent::UseItemAt(int SlotIndex)
{
	if (!Items[SlotIndex])
		return;
	
	if (TObjectPtr<IUsableItem> UsableItem = Cast<IUsableItem>(Items[SlotIndex]))
	{
		UsableItem->Use(GetOwner());
		Items[SlotIndex]->TryUse(1);
	}
	
	if (Items[SlotIndex]->IsEmpty())
		Items[SlotIndex] = nullptr;
}

void UInventoryComponent::DropItems(int SlotIndex, int Amount = 1)
{
	if (!Items[SlotIndex])
		return;
	
	for (int i = 0; i < Amount; ++i)
	{
		if (!Items[SlotIndex]->IsEmpty())
		{
			GetWorld()->SpawnActor<AActor>(Items[SlotIndex]->GetItemDataAsset()->ItemClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
			Items[SlotIndex]->TryUse(1);
		}
	}
	
	if (Items[SlotIndex]->IsEmpty())
		Items[SlotIndex] = nullptr;
}

void UInventoryComponent::Clear()
{
	Items.Empty();
	Items.SetNum(Size);
}

void UInventoryComponent::TransferItem(TObjectPtr<UInventoryComponent> InventoryComponent, TObjectPtr<UItemInstance> Item, int SlotIndex)
{
	//transfer item from inventory to another, or the same inventory
	//if item has the same type, we try to stack them
	//if there is rest, we add it to an empty slot
}

void UInventoryComponent::SwapItem(TObjectPtr<UInventoryComponent> InventoryComponent, TObjectPtr<UItemInstance> FromItem, TObjectPtr<UItemInstance> ToItem, int SlotIndex)
{
	//else we swap them
}

int UInventoryComponent::GetEmptySlot() const
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (!Items[i])
			return i;
	}
	
	return -1;
}

int UInventoryComponent::GetItemSlot(TObjectPtr<UItemDataAsset> Item) const
{
	if (!Item)
		return -1;
	
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i]->GetItemDataAsset() == Item)
		{
			return i;
		}
	}
	
	return -1;
}

int UInventoryComponent::GetStackableItemSlot(TObjectPtr<UItemDataAsset> Item) const
{
	if (!Item)
		return -1;
	
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i]->GetItemDataAsset() == Item)
		{
			if (Items[i]->GetStackNumber() < Items[i]->GetItemDataAsset()->StackLimit)
			{
				return i;
			}
		}
	}
	
	return -1;
}

bool UInventoryComponent::Contains(TObjectPtr<UItemDataAsset> Item, int StackNumber) const
{
	int Counter = 0;
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i]->GetItemDataAsset() == Item)
		{
			if (Items[i]->GetStackNumber() == StackNumber)
			{
				return true;
			}
			Counter += Items[i]->GetStackNumber();
		}
	}
	if (Counter >= StackNumber)
	{
		return true;
	}
	return false;
}

bool UInventoryComponent::IsSlotEmpty(int SlotIndex) const
{
	return !Items[SlotIndex];
}

bool UInventoryComponent::IsFull() const
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (!Items[i])
			return false;
	}
	
	return true;
}

