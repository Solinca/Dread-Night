#include "UI/PopUp/ItemNotification.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "TimerManager.h"
#include "Items/Data/ItemDataAsset.h"
#include "Items/Object/ItemInstance.h"

void UItemNotification::InitNotification(const UItemInstance* Data, const int Quantity)
{
	TextItemName->SetText(FText::FromName(Data->GetDataAsset()->ItemName));
	TextQuantity->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Quantity)));
	ImageIcon->SetBrushFromTexture(Data->GetDataAsset()->ItemIcon);

	if (FadeInAnim)
	{
		PlayAnimation(FadeInAnim);
	}

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			TimerHandleLifeSpan, 
			this, 
			&ThisClass::StartFadeOut, 
			3.0f, 
			false
		);
	}
}

void UItemNotification::NativeConstruct()
{
	Super::NativeConstruct();
    
	if (FadeOutAnim)
	{
		FWidgetAnimationDynamicEvent EndAnimEvent;
		EndAnimEvent.BindDynamic(this, &ThisClass::OnFadeOutFinished);
		BindToAnimationFinished(FadeOutAnim, EndAnimEvent);
	}
}

void UItemNotification::StartFadeOut()
{
	if (FadeOutAnim)
	{
		PlayAnimation(FadeOutAnim);
		return;
	}
	
	OnFadeOutFinished();
}

void UItemNotification::OnFadeOutFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleLifeSpan);
	RemoveFromParent();
}
