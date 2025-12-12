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

void UInventoryComponent::AddItem(UItemInstance* Item)
{
	if (!Item || IsFull())
		return;
	
	if (Contains(Item->GetDataAsset(), 1))
	{
		for (int i = 0; i < Items.Num(); ++i)
		{
			if (Items[i]->CanBeStackedWith(Item, UItemInstance::EStackMethod::SameType))
			{
				if (Items[i]->GetStackNumber() != Items[i]->GetDataAsset()->StackLimit)
				{
					Items[i]->TryStackWith(Item);
					OnItemModified.Broadcast(Items[i], i);
					
					if (Item->IsEmpty())
						return;
				}
			}
		}
	}
	
	if (Item->IsEmpty())
		return;
	
	int index = GetEmptySlot().GetValue();
	Items[GetEmptySlot().GetValue()] = Item;
	OnItemAdded.Broadcast(Item, index);
}

void UInventoryComponent::RemoveItemsByType(UItemDataAsset* Item, int Amount)
{
	if (!Item)
		return;
	
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i] && Items[i]->GetDataAsset() == Item)
		{
			//Decrement Stack
			if (Items[i]->GetStackNumber() - Amount < 0)
			{
				Amount -= Items[i]->GetStackNumber();
				Items[i]->TryRemove(Items[i]->GetStackNumber());
			}
			else
			{
				Items[i]->TryRemove(Amount);
			}
			
			//If stack is Empty, Free Inventory Slot
			if (Items[i]->IsEmpty())
			{
				Items[i] = nullptr;
				OnItemRemoved.Broadcast(i);
			}
			
			return;
		}
	}
}

void UInventoryComponent::RemoveItemsAt(int SlotIndex, int Amount)
{
	if (!Items[SlotIndex])
		return;
	
	Items[SlotIndex]->TryRemove(Amount);
	OnItemModified.Broadcast(Items[SlotIndex],SlotIndex);
	
	if (Items[SlotIndex]->IsEmpty())
	{
		Items[SlotIndex] = nullptr;
		OnItemRemoved.Broadcast(SlotIndex);
	}
}

void UInventoryComponent::UseItemByType(UItemDataAsset* Item)
{
	if (!Item)
		return;

	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i] && Items[i]->GetDataAsset() == Item)
		{
			Items[i]->TryUse(1,GetOwner());
				
			if (Items[i]->IsEmpty())
			{
				Items[i] = nullptr;
				OnItemRemoved.Broadcast(i);
			}
				
			return;
		}
		
	}
}

void UInventoryComponent::UseItemAt(int SlotIndex)
{
	if (!Items[SlotIndex])
		return;
	
	Items[SlotIndex]->TryUse(1,GetOwner());
	OnItemModified.Broadcast(Items[SlotIndex],SlotIndex);
	
	if (Items[SlotIndex]->IsEmpty())
	{
		Items[SlotIndex] = nullptr;
		OnItemRemoved.Broadcast(SlotIndex);
	}
}

void UInventoryComponent::DropItems(int SlotIndex, int Amount = 1)
{
	if (!Items[SlotIndex])
		return;
	
	for (int i = 0; i < Amount; ++i)
	{
		if (!Items[SlotIndex]->IsEmpty())
		{
			GetWorld()->SpawnActor<AActor>(Items[SlotIndex]->GetDataAsset()->ItemClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
			Items[SlotIndex]->TryRemove(1);
		}
	}
	
	if (Items[SlotIndex]->IsEmpty())
	{
		OnItemRemoved.Broadcast(SlotIndex);
		Items[SlotIndex] = nullptr;
	}
}

void UInventoryComponent::Clear()
{
	Items.Empty();
	Items.SetNum(Size);
	OnItemCleared.Broadcast();
}

void UInventoryComponent::TransferItem(UInventoryComponent* InventoryComponent, UItemInstance* Item, int SlotIndex)
{
	//transfer item from inventory to another, or the same inventory
	//if item has the same type, we try to stack them
	//if there is rest, we add it to an empty slot
}

void UInventoryComponent::SwapItem(UInventoryComponent* InventoryComponent, UItemInstance* FromItem, UItemInstance* ToItem, int SlotIndex)
{
	//else we swap them
}

TOptional<int> UInventoryComponent::GetEmptySlot() const
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (!Items[i])
			return i;
	}
	
	return NullOpt;
}

UItemInstance* UInventoryComponent::GetItemAtSlot(int SlotIndex) const
{
	return Items[SlotIndex];
}

UItemDataAsset* UInventoryComponent::GetItemTypeAtSlot(int SlotIndex) const
{
	if (!Items[SlotIndex])
		return nullptr;
	
	return Items[SlotIndex]->GetDataAsset();
}

TOptional<int> UInventoryComponent::GetItemSlot(UItemDataAsset* Item) const
{
	if (!Item)
		return NullOpt;
	
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i] && Items[i]->GetDataAsset() == Item)
		{
			return i;
		}
	}
	
	return NullOpt;
}

TOptional<int> UInventoryComponent::GetStackableItemSlot(UItemDataAsset* Item) const
{
	if (!Item)
		return NullOpt;
	
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i] && Items[i]->GetDataAsset() == Item)
		{
			if (Items[i]->GetStackNumber() < Items[i]->GetDataAsset()->StackLimit)
			{
				return i;
			}
		}
	}
	
	return NullOpt;
}

bool UInventoryComponent::Contains(UItemDataAsset* Item, int StackNumber) const
{
	int Counter = 0;
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i] && Items[i]->GetDataAsset() == Item)
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