#include "UI/Widgets/InventoryBackground.h"


void UInventoryBackground::AddChildToInventoryCanvas(UUserWidget* Widget) const
{
	InventoryCanvas->AddChild(Widget);
}

void UInventoryBackground::AddChildToOtherInventoryCanvas(UUserWidget* Widget) const
{
	OtherInventoryCanvas->AddChild(Widget);
}
