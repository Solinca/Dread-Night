#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventorySlider.generated.h"

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
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	TObjectPtr<USlider> GetSlider() const { return Slider; }
	TObjectPtr<UButton> GetValidateButton() const { return ValidateButton; }
	TObjectPtr<UButton> GetReturnButton() const { return ReturnButton; }
	TObjectPtr<UTextBlock> GetAmountText() const { return AmountText; }
};
