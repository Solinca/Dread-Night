#include "UI/Widgets/InventoryQuickAddSlot.h"
#include "Items/Data/ItemDataAsset.h"

void UInventoryQuickAddSlot::SetupMenu(UInventoryComponent* Inventory, UInventoryComponent* TargetInventory)
{
	if (!TargetInventory)
		return;
	
	VerticalBox->ClearChildren();
	
	for (int i = 0; i < TargetInventory->GetSize(); ++i)
	{
		UInventoryQuickAddButton* NewQuickAddButton = CreateWidget<UInventoryQuickAddButton>(this, QuickAddButtonClass);
	
		NewQuickAddButton->SetVisibility(ESlateVisibility::Collapsed);
		if (UItemInstance* Item = TargetInventory->GetItemAtSlot(i))
		{
			if (Inventory->GetItemAtSlot(i)->GetDataAsset()->Type == Item->GetDataAsset()->Type)
			{
				if (TargetInventory->GetStackableItemSlot(Item->GetDataAsset()))
				{
					NewQuickAddButton->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
		else if (TargetInventory->IsSlotEmpty(i))
		{
			NewQuickAddButton->SetVisibility(ESlateVisibility::Visible);
		}
		NewQuickAddButton->SetupButtonSlot(i);
		NewQuickAddButton->OnQuickAddButtonClicked.AddDynamic(this, &UInventoryQuickAddSlot::OnClicked);
		VerticalBox->AddChildToVerticalBox(NewQuickAddButton);
	}
}

void UInventoryQuickAddSlot::OnClicked(int Index)
{
	OnQuickActionPressedEvent.Broadcast(Index);
}
