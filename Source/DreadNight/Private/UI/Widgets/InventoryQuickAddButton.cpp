#include "UI/Widgets/InventoryQuickAddButton.h"

void UInventoryQuickAddButton::NativeConstruct()
{
	Super::NativeConstruct();
	QuickAddButton->OnClicked.AddDynamic(this, &UInventoryQuickAddButton::OnButtonClicked);
}

void UInventoryQuickAddButton::SetupButtonSlot(int Index)
{
	SlotIndex = Index;
	QuickAddText->SetText(FText::AsNumber(SlotIndex + 1));
}

void UInventoryQuickAddButton::OnButtonClicked()
{
	OnQuickAddButtonClicked.Broadcast(SlotIndex);
}
