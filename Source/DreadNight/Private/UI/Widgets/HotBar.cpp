#include "UI/Widgets/HotBar.h"
#include "Components/HorizontalBox.h"
#include "UI/Widgets/HotBarSlot.h"

void UHotBar::NativeConstruct()
{
	Super::NativeConstruct();

	SlotsArray.Empty();

	for (int32 i = 0; i < HotBarSlotCount; i++)
	{
		if (UHotBarSlot* SlotWidget = CreateWidget<UHotBarSlot>(this, HotBarSlotClass))
		{
			SlotsArray.Add(SlotWidget);

			SlotsContainer->AddChildToHorizontalBox(SlotWidget);
			
			if (HotBarKeys.IsValidIndex(i))
			{
				static constexpr int HotBarMaxSlots = 10;
				SlotWidget->SetShortcutText(FText::AsNumber((i + 1) % HotBarMaxSlots));
			}
		}
	}
}
