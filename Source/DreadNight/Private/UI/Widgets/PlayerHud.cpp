#include "UI/Widgets/PlayerHud.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "InteractableSystem/Interfaces/Interactable.h"
#include "InteractableSystem/Subsystems/InteractableSubsystem.h"
#include "UI/PopUp/ItemNotification.h"
#include "UI/Widgets/RadialProgressBarImage.h"
#include "UI/Widgets/SmartProgressBar.h"


void UPlayerHud::NativeConstruct()
{
	Super::NativeConstruct();

	if (UInteractableSubsystem* InteractableSubsystem = GetWorld()->GetSubsystem<UInteractableSubsystem>())
	{
		InteractableSubsystem->OnFocusChanged.AddDynamic(this, &ThisClass::UpdateInteractionText);
	}

	
}

void UPlayerHud::UpdateHealthBar(const float CurrentValue, const float MaxValue)
{
	HealthBar->UpdateValue(CurrentValue, MaxValue);
}

void UPlayerHud::UpdateStaminaBar(const float CurrentValue, const float MaxValue)
{
	StaminaBar->UpdateValue(CurrentValue, MaxValue);
}

void UPlayerHud::UpdateManaBar(const float CurrentValue, const float MaxValue)
{
	ManaBar->UpdateValue(CurrentValue, MaxValue);
}

void UPlayerHud::UpdateThirstRadialBarImage(const float CurrentValue, bool IsState)
{
	if (ThirstRadialBarImage)
	{
		ThirstRadialBarImage->UpdateValue(CurrentValue);
	}
}

void UPlayerHud::UpdateHungerRadialBarImage(const float CurrentValue, bool IsState)
{
	if (HungerRadialBarImage)
	{
		HungerRadialBarImage->UpdateValue(CurrentValue);
	}
}

void UPlayerHud::UpdateMentalRadialBarImage(const float CurrentValue, bool IsState)
{
	if (MentalRadialBarImage)
	{
		MentalRadialBarImage->UpdateValue(CurrentValue);
	}
}

void UPlayerHud::UpdateFearRadialBarImage(const float CurrentValue, bool IsState)
{
	if (FearRadialBarImage)
	{
		FearRadialBarImage->UpdateValue(CurrentValue);
	}
}

void UPlayerHud::UpdateInteractionText(AActor* NewInteractable)
{
	if (IsValid(NewInteractable) && NewInteractable->Implements<UInteractable>())
	{
		const FText Text = IInteractable::Execute_GetInteractionPromptText(NewInteractable);
    
		InteractionText->SetText(Text);
		
		InformationTextHorizontalBox->SetVisibility(ESlateVisibility::HitTestInvisible);
		return;
	}
	
	InformationTextHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHud::AddItemNotification(const UItemInstance* Data, const int Quantity, const TSubclassOf<UItemNotification> WidgetToInstantiate)
{
	if (UItemNotification* NewItemNotification = CreateWidget<UItemNotification>(this, WidgetToInstantiate))
	{
		NotificationContainer->InsertChildAt(0, NewItemNotification);
        
		NewItemNotification->InitNotification(Data, Quantity);
	}
}