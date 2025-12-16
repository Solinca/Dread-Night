#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryInfo.generated.h"

UCLASS()
class DREADNIGHT_API UInventoryInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> ItemInfoButton;
	
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemInfoText;
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	TObjectPtr<UButton> GetItemInfoButton() const { return ItemInfoButton; }
	TObjectPtr<UTextBlock> GetItemInfoText() const { return ItemInfoText; }
};
