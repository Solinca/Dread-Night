#include "UI/Widgets/InventorySlot.h"


void UInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	ItemButton->OnClicked.AddDynamic(this, &UInventorySlot::SlotAction);
}

void UInventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Unhandled();
	
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		HasRightClicked = true;
		ItemButton->OnClicked.Broadcast();
	}
	else
	{
		HasRightClicked = false;
	}
	return Reply;
}

void UInventorySlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& InMouseEvent)
{
	IsMouseOver = true;
	InfoAction();
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	IsMouseOver = false;
}

void UInventorySlot::SetItemImage(UTexture2D* Texture)
{
	ItemImage->SetBrushFromTexture(Texture);
}

void UInventorySlot::SetStackText(int Stack)
{
	if (Stack > 0) 
	{
		StackText->SetText(FText::FromString(FString::FromInt(Stack)));
		StackText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		StackText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventorySlot::SetupSlot(UInventoryComponent* OwningInventory, UInventoryComponent* TargetInventory, int Index)
{
	BindInventoryComponent = OwningInventory;
	BindTargetInventoryComponent = TargetInventory;
	SlotIndex = Index;
}

void UInventorySlot::SlotAction()
{
	if (HasRightClicked)
	{
		OnItemActionCreated.Broadcast(SlotIndex);
	}
}

void UInventorySlot::InfoAction()
{
	if (HasRightClicked)
		return;
	
	if (IsMouseOver)
	{
		OnItemInfoCreated.Broadcast(SlotIndex);
	}
	else
	{
		OnItemInfoRemoved.Broadcast();
	}
}

void UInventorySlot::BindToInventory(UInventoryComponent* InventoryComponent)
{
	BindInventoryComponent = InventoryComponent;
}

void UInventorySlot::Reset(const FSlateBrush& Brush)
{
	ItemImage->SetBrush(Brush);
	StackText->SetText(FText::FromString(FString::FromInt(0)));
}

const FSlateBrush& UInventorySlot::GetImageBrush() const
{
	return ItemImage->GetBrush();
}