#include "UI/Widgets/Inventory.h"

#include "IDetailTreeNode.h"
#include "Items/Data/ItemDataAsset.h"

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
		TempSlot->SetSlotIndex(i);
		TempSlot->BindToInventory(BindInventoryComponent);
		TempSlot->OnItemActionCreated.AddDynamic(this, &UInventory::OnItemActionCreated);
		InventoryWrapBox->AddChildToWrapBox(TempSlot);
	}
}

void UInventory::OnItemAdded(UItemInstance* Item, int SlotIndex)
{
	if (UInventorySlot* TempSlot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex)))
	{
		TempSlot->SetItemImage(Item->GetItemDataAsset()->ItemIcon);
		TempSlot->SetStackText(Item->GetStackNumber());
	}
}

void UInventory::OnItemRemoved(int SlotIndex)
{
	if (UInventorySlot* TempSlot = Cast<UInventorySlot>(InventoryWrapBox->GetChildAt(SlotIndex)))
	{
		TempSlot =  CreateWidget<UInventorySlot>(this, InventorySlotClass);
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

void UInventory::OnItemActionCreated(int SlotIndex)
{
	if (InventoryAction)
		InventoryAction->RemoveFromParent();
	
	InventoryAction = CreateWidget<UInventoryAction>(this, InventoryActionClass);
	InventoryAction->InventoryComponent = BindInventoryComponent;
	InventoryAction->SetSlotIndex(SlotIndex);
	FVector2d MousePos;
	GetOwningPlayer()->GetMousePosition(MousePos.X, MousePos.Y);
	InventoryAction->SetRenderTranslation(MousePos);
	InventoryAction->AddToViewport();
}
