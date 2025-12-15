#include "UI/Widgets/HotBar.h"
#include "Components/HorizontalBox.h"
#include "UI/Widgets/HotBarSlot.h"

void UHotBar::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 0; i < InventoryWrapBox->GetWrapSize(); i++)
	{
		if (!ensureMsgf(InventorySlotClass, TEXT("HotBarSlotClass is null in %s! Please assign the widget class in the Blueprint."), *GetName()))
		{
			return;
		}
		
		UHotBarSlot* SlotWidget = CreateWidget<UHotBarSlot>(this, InventorySlotClass);
		
		InventoryWrapBox->AddChildToWrapBox(SlotWidget);
	
		static constexpr int HotBarMaxSlots = 10;
		SlotWidget->SetShortcutText(FText::AsNumber((i + 1) % HotBarMaxSlots));
	}
}
