#include "UI/Widgets/InventoryQuickAddSlot.h"

#include "Components/Button.h"

void UInventoryQuickAddSlot::NativePreConstruct()
{
}

void UInventoryQuickAddSlot::NativeConstruct()
{
}

void UInventoryQuickAddSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryQuickAddSlot::SetupMenu(UInventoryComponent* Inventory)
{
	if (!Inventory)
		return;
	
	TargetInventory = Inventory;
	VerticalBox->ClearChildren();
	
	for (int i = 0; i < Inventory->GetSize(); ++i)
	{
		QuickAddButton = CreateWidget<UInventoryQuickAddButton>(this, QuickAddButtonClass);
	
		QuickAddButton->SetVisibility(ESlateVisibility::Collapsed);
		if (UItemInstance* Item = Inventory->GetItemAtSlot(i))
		{
			if (Inventory->GetStackableItemSlot(Item->GetDataAsset()))
			{
				QuickAddButton->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else if (Inventory->IsSlotEmpty(i))
		{
			QuickAddButton->SetVisibility(ESlateVisibility::Visible);
		}
		
		QuickAddButton->GetQuickAddButton()->OnClicked.AddDynamic(this, &UInventoryQuickAddSlot::OnClicked);
		QuickAddButton->SetQuickAddText(FText::FromString(FString::FromInt(i+1)));
		VerticalBox->AddChildToVerticalBox(QuickAddButton);
	}
}

void UInventoryQuickAddSlot::OnClicked()
{
	OnQuickActionPressedEvent.Broadcast();
}
