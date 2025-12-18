#include "UI/Widgets/InventoryQuickAddSlot.h"

#include "Components/Button.h"

void UInventoryQuickAddSlot::NativePreConstruct()
{
}

void UInventoryQuickAddSlot::NativeConstruct()
{
}

void UInventoryQuickAddSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryQuickAddSlot::SetSize(int Size)
{
	VerticalBox->ClearChildren();
	
	for (int i = 0; i < Size; ++i)
	{
		UInventoryQuickAddButton* QuickAddButton = CreateWidget<UInventoryQuickAddButton>(this, QuickAddButtonClass);
		QuickAddButton->GetQuickAddButton()->OnClicked.AddDynamic(this, &UInventoryQuickAddSlot::OnClicked);
		QuickAddButton->SetQuickAddText(FText::FromString(FString::FromInt(i+1)));
		VerticalBox->AddChildToVerticalBox(QuickAddButton);
	}
}

void UInventoryQuickAddSlot::OnClicked()
{
	OnQuickActionPressedEvent.Broadcast();
}
