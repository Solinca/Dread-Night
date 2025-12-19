#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventorySlider.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSliderValidatedEventSignature, int, Amount);

UCLASS()
class DREADNIGHT_API UInventorySlider : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UImage> Image;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<USlider> Slider;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> ValidateButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> ReturnButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> AmountText;
	
	UFUNCTION()
	void OnSliderValueChanged(float Value);
	UFUNCTION()
	void OnValidateButtonClicked();
	UFUNCTION()
	void OnReturnButtonClicked();
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void SetupSlider(int MaxSize);
	
	FOnSliderValidatedEventSignature OnSliderValidated;
	
	USlider* GetSlider() const { return Slider; }
	UButton* GetValidateButton() const { return ValidateButton; }
	UButton* GetReturnButton() const { return ReturnButton; }
	UTextBlock* GetAmountText() const { return AmountText; }
};
