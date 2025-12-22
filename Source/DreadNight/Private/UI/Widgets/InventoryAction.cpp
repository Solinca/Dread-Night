#include "UI/Widgets/InventoryAction.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Items/Data/ItemDataAsset.h"

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
	
	if (InventoryQuickAddSlot)
		InventoryQuickAddSlot->RemoveFromParent();
	
	UInventoryQuickAddSlot* QuickAddWidget = CreateWidget<UInventoryQuickAddSlot>(this, InventoryQuickAddWidgetClass);
	if (TargetInventoryComponent->GetName() == "HotBarInventoryComponent" && QuickAddWidget)
	{
		FVector2D PixelPos;
		FVector2D ViewportPos;
		float OffsetX = 0.1f;
		float OffsetY = -190.f;
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), GetCachedGeometry(), FVector2D(GetCachedGeometry().GetLocalSize().X, 0.f), PixelPos, ViewportPos);
		
		QuickAddWidget->SetupMenu(TargetInventoryComponent);
		QuickAddWidget->OnQuickActionPressedEvent.AddDynamic(this, &UInventoryAction::OnQuickActionPressed);
		QuickAddWidget->AddToViewport();
		QuickAddWidget->SetPositionInViewport(ViewportPos + FVector2D(GetCachedGeometry().GetLocalSize().X * OffsetX, OffsetY), false);
		
		InventoryQuickAddSlot = QuickAddWidget;
		return;
	}
	
	if (UItemInstance* ItemTransfered = InventoryComponent->GetItemAtSlot(SlotIndex))
	{
		InventoryComponent->TransferItem(TargetInventoryComponent, ItemTransfered);
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
		
		SetVisibility(ESlateVisibility::Collapsed);
		return;
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
	UItemInstance* ItemFrom = InventoryComponent->GetItemAtSlot(SlotIndex);
	UItemInstance* ItemTo = TargetInventoryComponent->GetItemAtSlot(Index - 1);
	if (ItemFrom && ItemTo && ItemFrom->GetDataAsset()->Type != ItemTo->GetDataAsset()->Type)
	{
		InventoryComponent->SwapItem(TargetInventoryComponent, ItemFrom, ItemTo, Index-1);
	}
	else
	{
		InventoryComponent->TransferItemAt(TargetInventoryComponent, ItemFrom, Index-1);
	}
	
	InventoryQuickAddSlot->RemoveFromParent();
	RemoveFromParent();
}
