#include "UI/Widgets/InventoryAction.h"

void UInventoryAction::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UInventoryAction::NativeConstruct()
{
	Super::NativeConstruct();
	
	UseButton->OnClicked.AddDynamic(this, &UInventoryAction::OnUsePressed);
	TransferButton->OnClicked.AddDynamic(this, &UInventoryAction::OnTransferPressed);
	DropButton->OnClicked.AddDynamic(this, &UInventoryAction::OnDropPressed);
	RemoveButton->OnClicked.AddDynamic(this, &UInventoryAction::OnRemovePressed);
}

void UInventoryAction::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryAction::OnUsePressed()
{
	InventoryComponent->UseItemAt(SlotIndex);
	RemoveFromParent();
}

void UInventoryAction::OnTransferPressed()
{
	RemoveFromParent();
}

void UInventoryAction::OnDropPressed()
{
	InventoryComponent->DropItems(SlotIndex, 1);
	RemoveFromParent();
}

void UInventoryAction::OnRemovePressed()
{
	InventoryComponent->RemoveItemsAt(SlotIndex, 1);
	RemoveFromParent();
}

