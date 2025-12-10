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

void UInventorySlot::SetItemImage(UTexture2D* Texture)
{
	ItemImage->SetBrushFromTexture(Texture);
}

void UInventorySlot::SetStackText(int Stack)
{
	StackText->SetText(FText::FromString(FString::FromInt(Stack)));
}

void UInventorySlot::SlotAction()
{
	if (HasRightClicked)
		OnItemActionCreated.Broadcast(SlotIndex);
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
