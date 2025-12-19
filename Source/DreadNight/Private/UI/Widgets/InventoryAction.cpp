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

void UInventoryAction::NativeDestruct()
{
	if (InventoryQuickAddSlot)
	{
		InventoryQuickAddSlot->RemoveFromParent();
	}
	if (InventorySlider)
	{
		InventorySlider->RemoveFromParent();
	}
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
	
	if (TargetInventoryComponent->GetName() == "HotBarInventoryComponent")
	{
		if (UInventoryQuickAddSlot* QuickAddWidget = CreateWidget<UInventoryQuickAddSlot>(this, InventoryQuickAddWidgetClass))
		{
			QuickAddWidget->SetupMenu(InventoryComponent,TargetInventoryComponent);
			
			FVector2D WidgetPos = GetCachedGeometry().GetAbsolutePosition();
			float OffsetX = 240.f;
			float OffsetY = 30.f;
			QuickAddWidget->SetPositionInViewport(FVector2D(WidgetPos.X - QuickAddWidget->GetVerticalBox()->GetDesiredSize().X / 2 - OffsetX,
			 										WidgetPos.Y - QuickAddWidget->GetVerticalBox()->GetDesiredSize().Y / 2 - OffsetY));
			
			QuickAddWidget->OnQuickActionPressedEvent.AddDynamic(this, &UInventoryAction::OnQuickActionPressed);
			InventoryQuickAddSlot = QuickAddWidget;
			QuickAddWidget->AddToViewport();
		}
	}
	else
	{
		if (UItemInstance* ItemTransfered = InventoryComponent->GetItemAtSlot(SlotIndex))
		{
			InventoryComponent->TransferItem(TargetInventoryComponent, ItemTransfered);
		}
		RemoveFromParent();
	}
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
	}
	RemoveFromParent();
}

void UInventoryAction::OnRemoveAmountSelected(int Amount)
{
	if (Amount)
	{
		InventoryComponent->RemoveItemsAt(SlotIndex, Amount);
	}
}

void UInventoryAction::OnQuickActionPressed(int Index)
{
	if (UItemInstance* ItemTransfered = InventoryComponent->GetItemAtSlot(SlotIndex))
	{
		InventoryComponent->TransferItemAt(TargetInventoryComponent, ItemTransfered, Index-1);
	}
	InventoryQuickAddSlot->RemoveFromParent();
	RemoveFromParent();
}
