#include "UI/Widgets/HotBarSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/Data/ItemDataAsset.h"
#include "Items/Object/ItemInstance.h"


void UHotBarSlot::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateSlot(nullptr, 0);
}

FReply UHotBarSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UHotBarSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (!CurrentItemInstance)
	{
		return;
	}
	
	// TODO all the logic of drag from (UDragDropOperation) ?
}

bool UHotBarSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	// TODO reception logic (Swap or assign ?
	return true;
}

void UHotBarSlot::UpdateSlot(UItemInstance* InItemInstance, const int32 InQuantity)
{
	CurrentItemInstance = InItemInstance;
	CurrentQuantity = InQuantity;

	if (CurrentItemInstance)
	{
		const UItemDataAsset* ItemDataAsset = CurrentItemInstance->GetDataAsset();

		ItemIcon->SetBrushFromTexture(ItemDataAsset->ItemIcon);
		ItemIcon->SetVisibility(ESlateVisibility::Visible);

		if (CurrentQuantity > 1)
		{
			StackText->SetText(FText::AsNumber(CurrentQuantity));
			StackText->SetVisibility(ESlateVisibility::Visible);
			return;
		}
		
		StackText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	//ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	StackText->SetVisibility(ESlateVisibility::Hidden);
}

void UHotBarSlot::SetShortcutText(const FText& InText)
{
	ShortcutText->SetText(InText);
}
