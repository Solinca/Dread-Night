#include "UI/Widgets/Inventory.h"
#include "Items/Data/ItemDataAsset.h"
#include "Items/Data/ItemGameplayTag.h"
#include "Blueprint/WidgetLayoutLibrary.h"

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
		UInventorySlot* TempSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		
		TempSlot->SetupSlot(BindInventoryComponent,BindTargetInventoryComponent, i);
		TempSlot->SetImageColor(EmptyInventorySlot);
		
		if (UItemInstance* Item = BindInventoryComponent->GetItemAtSlot(i))
		{
			TempSlot->SetItemImage(Item->GetDataAsset()->ItemIcon);
			TempSlot->SetStackText(Item->GetStackNumber());
			TempSlot->SetImageColor(UsedInventorySlot);
		}
		
		TempSlot->OnItemInfoCreated.AddDynamic(this, &UInventory::OnItemInfoCreated);
		TempSlot->OnItemInfoRemoved.AddDynamic(this, &UInventory::OnItemInfoRemoved);
		TempSlot->OnItemActionCreated.AddDynamic(this, &UInventory::OnItemActionCreated);
		InventoryWrapBox->AddChildToWrapBox(TempSlot);
	}
}

void UInventory::OnItemAdded(UItemInstance* Item, int SlotIndex)
{
	if (UInventorySlot* TempSlot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex)))
	{
		TempSlot->SetItemImage(Item->GetDataAsset()->ItemIcon);
		TempSlot->SetStackText(Item->GetStackNumber());
		TempSlot->SetImageColor(UsedInventorySlot);		
	}
}

void UInventory::OnItemRemoved(int SlotIndex)
{
	if (UInventorySlot* TempSlot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex)))
	{
		UInventorySlot* SlotToClone = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		TempSlot->Reset(SlotToClone->GetImageBrush());
		TempSlot->SetImageColor(EmptyInventorySlot);
	}
}

void UInventory::OnItemModified(UItemInstance* Item, int SlotIndex)
{
	if (UInventorySlot* TempSlot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex)))
	{
		TempSlot->SetStackText(Item->GetStackNumber());
	}
}

void UInventory::OnItemsCleared()
{
	SetSize(InventoryWrapBox->GetWrapSize());
}

void UInventory::BindToInventory(UInventoryComponent* InventoryComponent)
{
	BindInventoryComponent = InventoryComponent;
	
	BindInventoryComponent->OnItemAdded.AddDynamic(this, &UInventory::OnItemAdded);
	BindInventoryComponent->OnItemRemoved.AddDynamic(this, &UInventory::OnItemRemoved);
	BindInventoryComponent->OnItemModified.AddDynamic(this, &UInventory::OnItemModified);
	BindInventoryComponent->OnItemCleared.AddDynamic(this, &UInventory::OnItemsCleared);
	
	SetSize(InventoryComponent->GetSize());
}

void UInventory::BindTargetInventory(UInventoryComponent* InventoryComponent)
{
	BindTargetInventoryComponent = InventoryComponent;
}

void UInventory::OnItemActionCreated(int SlotIndex)
{
	if (!BindInventoryComponent->GetItemAtSlot(SlotIndex))
		return;
	
	if (GlobalInventoryAction.IsValid() && GlobalInventoryAction != InventoryAction)
		GlobalInventoryAction->RemoveFromParent();
	
	if (InventoryAction)
		InventoryAction->RemoveFromParent();
	
	if (InventoryInfoWidget)
		OnItemInfoRemoved();
	
	UInventorySlot* ClickedSlot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex));
	if (!ClickedSlot)
		return;
	
	const FVector2d MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	OffsetX = ViewportSize.X * 0.06f;
	OffsetY = ViewportSize.Y * 0.05f;
	Offset = FVector2D(OffsetX, OffsetY);

	Offset.Y = (MousePos.Y >= ViewportSize.Y / 1.5f ? -OffsetY : OffsetY);
	
	InventoryAction = CreateWidget<UInventoryAction>(this, InventoryActionClass);
	InventoryAction->SetupAction(BindInventoryComponent, BindTargetInventoryComponent, SlotIndex);
	InventoryAction->AddToViewport();
	InventoryAction->SetPositionInViewport(MousePos + Offset, false);
	GlobalInventoryAction = InventoryAction;
	
	UItemInstance* ItemData = BindInventoryComponent->GetItemAtSlot(SlotIndex);
	if (IUsableItem* UsableItem = Cast<IUsableItem>(ItemData))
	{
		InventoryAction->GetUseButton()->SetVisibility(ESlateVisibility::Visible);
		InventoryAction->GetUseText()->SetText(FText::FromName(UsableItem->GetActionName()));
		
		ESlateVisibility TransferButtonVisibility = ESlateVisibility::Collapsed;
		if (!ItemData->GetDataAsset()->Type.MatchesTag(GT_Item_Armor) && !BindTargetInventoryComponent->IsFull())
		{
			TransferButtonVisibility = ESlateVisibility::Visible;
		}
		
		ESlateVisibility UseButtonVisibility = ESlateVisibility::Collapsed;
		if (ItemData->GetDataAsset()->Type.MatchesTag(GT_Item_Armor) && 
			BindInventoryComponent->GetName() == "InventoryComponent")
		{
			UseButtonVisibility = ESlateVisibility::Visible;
		}
		
		ESlateVisibility RemoveButtonVisibility = ESlateVisibility::Collapsed;
		if (BindInventoryComponent->GetName() != "HotbarInventoryComponent")
		{
			RemoveButtonVisibility = ESlateVisibility::Visible;
		}
		
		InventoryAction->GetUseButton()->SetVisibility(UseButtonVisibility);
		InventoryAction->GetTransferButton()->SetVisibility(TransferButtonVisibility);
		InventoryAction->GetRemoveButton()->SetVisibility(RemoveButtonVisibility);
	}
	else
	{
		InventoryAction->GetUseButton()->SetVisibility(ESlateVisibility::Collapsed);
		ESlateVisibility TransferButtonVisibility = BindTargetInventoryComponent.GetName() != "HotBarInventoryComponent" ? 
													ESlateVisibility::Visible : ESlateVisibility::Collapsed;
		
		InventoryAction->GetTransferButton()->SetVisibility(TransferButtonVisibility);
	}
}

void UInventory::OnItemInfoCreated(int SlotIndex)
{
	if (!ItemInfoWidgetClass)
		return;
	
	if (InventoryInfoWidget)
		InventoryInfoWidget->RemoveFromParent();
	
	const FVector2d MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	OffsetX = ViewportSize.X * 0.06f;
	OffsetY = ViewportSize.Y * 0.05f;
	Offset = FVector2D(OffsetX, OffsetY);

	Offset.Y = (MousePos.Y >= ViewportSize.Y / 1.5f ? -OffsetY : OffsetY);

	InventoryInfoWidget = CreateWidget<UInventoryInfo>(this, ItemInfoWidgetClass);
	InventoryInfoWidget->AddToViewport();
	InventoryInfoWidget->SetPositionInViewport(MousePos + Offset, false);
	
	InventoryInfoWidget->GetItemInfoButton()->SetVisibility(ESlateVisibility::Hidden);
	if (UItemInstance* ItemData = BindInventoryComponent->GetItemAtSlot(SlotIndex))
	{
		InventoryInfoWidget->GetItemInfoButton()->SetVisibility(ESlateVisibility::Visible);
		InventoryInfoWidget->GetItemInfoText()->SetText(FText::FromName(ItemData->GetDataAsset()->ItemName));
	}
}

void UInventory::OnItemInfoRemoved()
{
	if (InventoryInfoWidget)
		InventoryInfoWidget->RemoveFromParent();
}

void UInventory::RemoveItemAction()
{
	if (InventoryAction)
		InventoryAction->RemoveFromParent();
}