#include "UI/Widgets/InventoryQuickAddSlot.h"

void UInventoryQuickAddSlot::SetupMenu(UInventoryComponent* TargetInventory)
{
	if (!TargetInventory)
		return;
	
	VerticalBox->ClearChildren();
	
	for (int i = 0; i < TargetInventory->GetSize(); ++i)
	{
		UInventoryQuickAddButton* NewQuickAddButton = CreateWidget<UInventoryQuickAddButton>(this, QuickAddButtonClass);
		NewQuickAddButton->SetupButtonSlot(i + 1);
		NewQuickAddButton->OnQuickAddButtonClicked.AddDynamic(this, &UInventoryQuickAddSlot::OnClicked);
		VerticalBox->AddChildToVerticalBox(NewQuickAddButton);
	}
}

void UInventoryQuickAddSlot::OnClicked(int Index)
{
	OnQuickActionPressedEvent.Broadcast(Index);
}
