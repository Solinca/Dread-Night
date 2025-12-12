#include "UI/PopUp/GlobalPopUp.h"

#include "Components/TextBlock.h"

void UGlobalPopUp::SpawnPopUp(const FText& InText)
{
	if (!TopViewportText)
	{
		return;
	}
	
	TopViewportText->SetText(InText);
	
	if (!ConstructAnimation)
	{
		return;
	}
	
	PlayAnimation(ConstructAnimation);

	FWidgetAnimationDynamicEvent AnimFinishedDelegate;
	AnimFinishedDelegate.BindDynamic(this, &UUserWidget::RemoveFromParent);

	BindToAnimationFinished(ConstructAnimation, AnimFinishedDelegate);
}