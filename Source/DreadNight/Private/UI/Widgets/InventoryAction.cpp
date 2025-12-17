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
	RemoveButton->OnClicked.AddDynamic(this, &UInventoryAction::OnRemovePressed);
}

void UInventoryAction::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryAction::SetupAction(UInventoryComponent* OwningInventory, UInventoryComponent* TargetInventory, int Index)
{
	InventoryComponent = OwningInventory;
	TargetInventoryComponent = TargetInventory;
	SlotIndex = Index;
}

void UInventoryAction::OnUsePressed()
{
	InventoryComponent->UseItemAt(SlotIndex);
	RemoveFromParent();
}

void UInventoryAction::OnTransferPressed()
{
	if (!TargetInventoryComponent)
	{
		RemoveFromParent();
		return;
	}
	
	if (UItemInstance* ItemTransfered = InventoryComponent->GetItemAtSlot(SlotIndex))
	{
		InventoryComponent->TransferItem(TargetInventoryComponent, ItemTransfered, TargetInventoryComponent->GetEmptySlot().GetValue());
	}
	
	RemoveFromParent();
}

void UInventoryAction::OnRemovePressed()
{
	if (UItemInstance* Item = InventoryComponent->GetItemAtSlot(SlotIndex))
	{
		if (Item->GetStackNumber() == 1)
		{
			InventoryComponent->RemoveItemsAt(SlotIndex, 1);
			RemoveFromParent();
			return;
		}
		
		if (!InventorySliderWidgetClass)
			return;
		
		InventorySlider = CreateWidget<UInventorySlider>(this, InventorySliderWidgetClass);
		InventorySlider->SetupSlider(Item->GetStackNumber());
		InventorySlider->OnSliderValidated.AddDynamic(this, &UInventoryAction::OnRemoveAmountSelected);
		InventorySlider->AddToViewport();
		RemoveFromParent();
	}
}

void UInventoryAction::OnRemoveAmountSelected(int Amount)
{
	if (Amount)
	{
		InventoryComponent->RemoveItemsAt(SlotIndex, Amount);
	}
}