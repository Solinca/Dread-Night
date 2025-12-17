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
		
		if (UHotBarSlot* SlotWidget = Cast<UHotBarSlot>(InventoryWrapBox->GetChildAt(i)))
		{ 
			SlotWidget->SetShortcutText(FText::AsNumber(i + 1));
		}
	}
}

void UHotBar::SetHelmetBrush(UTexture2D* HelmetTexture) const
{
	HelmetImage->SetBrushFromTexture(HelmetTexture);
}

void UHotBar::SetArmorBrush(UTexture2D* ArmorTexture) const
{
	ArmorImage->SetBrushFromTexture(ArmorTexture);
}

void UHotBar::SetWeaponBrush(UTexture2D* WeaponTexture) const
{
	WeaponImage->SetBrushFromTexture(WeaponTexture);
}
