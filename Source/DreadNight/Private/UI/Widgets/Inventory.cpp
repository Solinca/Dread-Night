// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Inventory.h"

#include "Items/Data/ItemDataAsset.h"

void UInventory::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventory::SetSize(int Size)
{
	InventoryWrapBox->ClearChildren();
	for (int i = 0; i < Size; i++)
	{
		UInventorySlot* Slot = CreateWidget<UInventorySlot>(this, InventorySlot);
		InventoryWrapBox->AddChildToWrapBox(Slot);
	}
}

void UInventory::OnItemAdded(UItemInstance* Item, int SlotIndex)
{
	if (UInventorySlot* Slot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex)))
	{
		Slot->SetItemImage(Item->GetItemDataAsset()->ItemIcon);
		Slot->SetStackText(Item->GetStackNumber());
	}
}

void UInventory::OnItemRemoved(int SlotIndex)
{
	if (UInventorySlot* Slot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex)))
	{
		Slot =  CreateWidget<UInventorySlot>(this, InventorySlot);
	}
}

void UInventory::OnItemModified(UItemInstance* Item, int SlotIndex)
{
	if (UInventorySlot* Slot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex)))
	{
		Slot->SetStackText(Item->GetStackNumber());
	}
}

void UInventory::OnItemsCleared()
{
	SetSize(InventoryWrapBox->GetWrapSize());
}
