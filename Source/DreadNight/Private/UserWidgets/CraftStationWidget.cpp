#include "UserWidgets/CraftStationWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCraftStationWidget::NativeConstruct()
{
	if (CraftButton)
		CraftButton->OnClicked.AddDynamic(this, &UCraftStationWidget::OnCraftButtonClicked);

	if (QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &UCraftStationWidget::OnQuitButtonClicked);
}

void UCraftStationWidget::OnCraftButtonClicked()
{

}

void UCraftStationWidget::OnQuitButtonClicked()
{

}
