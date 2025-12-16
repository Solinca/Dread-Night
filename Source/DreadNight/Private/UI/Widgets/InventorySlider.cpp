#include "UI/Widgets/InventorySlider.h"

void UInventorySlider::NativeConstruct()
{
	Super::NativeConstruct();
	
	Slider->OnValueChanged.AddDynamic(this, &UInventorySlider::OnSliderValueChanged);
	ValidateButton->OnClicked.AddDynamic(this, &UInventorySlider::OnValidateButtonClicked);
	ReturnButton->OnClicked.AddDynamic(this, &UInventorySlider::OnReturnButtonClicked);
}

void UInventorySlider::SetupSlider(int MaxSize)
{
	int MaxAmount = FMath::Max(1, MaxSize);
	
	Slider->SetMinValue(1.0f);
	Slider->SetMaxValue(static_cast<float>(MaxAmount));
	Slider->SetValue(1.0f);
	
	AmountText->SetText(FText::FromString("1"));
}

void UInventorySlider::OnSliderValueChanged(float Value)
{
	AmountText->SetText(FText::FromString(FString::FromInt(Slider->GetValue())));
}

void UInventorySlider::OnValidateButtonClicked()
{
	OnSliderValidated.Broadcast(Slider->GetValue());
	RemoveFromParent();
}

void UInventorySlider::OnReturnButtonClicked()
{
	RemoveFromParent();
}