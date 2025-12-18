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
	
	SetArmorImagesVisibility(ESlateVisibility::Hidden);
}

void UHotBar::SetHelmetBrush(UTexture2D* HelmetTexture) const
{
	HelmetImage->SetBrushFromTexture(HelmetTexture);
}

void UHotBar::SetChestBrush(UTexture2D* ChestTexture) const
{
	ChestImage->SetBrushFromTexture(ChestTexture);
}

void UHotBar::SetPantBrush(UTexture2D* PantTexture) const
{
	PantImage->SetBrushFromTexture(PantTexture);
}

void UHotBar::SetGloveBrush(UTexture2D* GloveTexture) const
{
	GloveImage->SetBrushFromTexture(GloveTexture);
}

void UHotBar::SetArmorImagesVisibility(ESlateVisibility ArmorImagesVisibility) const
{
	HelmetImage->SetVisibility(ArmorImagesVisibility);	
	ChestImage->SetVisibility(ArmorImagesVisibility);	
	PantImage->SetVisibility(ArmorImagesVisibility);	
	GloveImage->SetVisibility(ArmorImagesVisibility);
}
