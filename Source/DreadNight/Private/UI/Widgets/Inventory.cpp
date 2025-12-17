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
	
	if (InventoryAction)
		InventoryAction->RemoveFromParent();
	
	
	if (InventoryInfoWidget)
		OnItemInfoRemoved();
	
	UInventorySlot* ClickedSlot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex));
	if (!ClickedSlot)
		return;
	
	
	InventoryAction = CreateWidget<UInventoryAction>(this, InventoryActionClass);
	InventoryAction->SetupAction(BindInventoryComponent, BindTargetInventoryComponent, SlotIndex);
	
	FVector2d MousePos;
	GetOwningPlayer()->GetMousePosition(MousePos.X, MousePos.Y);
	InventoryAction->SetRenderTranslation(MousePos);
	InventoryAction->AddToViewport();
	
	UItemInstance* ItemData = BindInventoryComponent->GetItemAtSlot(SlotIndex);
	if (IUsableItem* UsableItem = Cast<IUsableItem>(ItemData))
	{
		InventoryAction->GetUseButton()->SetVisibility(ESlateVisibility::Visible);
		InventoryAction->GetUseText()->SetText(FText::FromName(UsableItem->GetActionName()));
	}
	else
	{
		InventoryAction->GetUseButton()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventory::OnItemInfoCreated(int SlotIndex)
{
	if (!ItemInfoWidgetClass)
		return;
	
	if (InventoryInfoWidget)
		InventoryInfoWidget->RemoveFromParent();
	
	FVector2d MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	InventoryInfoWidget = CreateWidget<UInventoryInfo>(this, ItemInfoWidgetClass);
	InventoryInfoWidget->SetRenderTranslation(MousePos);
	InventoryInfoWidget->AddToViewport();
	InventoryInfoWidget->SetDesiredSizeInViewport(FVector2D(100, 60));
	
	if (UItemInstance* ItemData = BindInventoryComponent->GetItemAtSlot(SlotIndex))
	{
		InventoryInfoWidget->GetItemInfoButton()->SetVisibility(ESlateVisibility::Visible);
		InventoryInfoWidget->GetItemInfoText()->SetText(FText::FromName(ItemData->GetDataAsset()->ItemName));
	}
	else
	{
		InventoryInfoWidget->GetItemInfoButton()->SetVisibility(ESlateVisibility::Hidden);
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


