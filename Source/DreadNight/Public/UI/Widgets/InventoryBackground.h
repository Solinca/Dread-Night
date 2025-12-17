#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "InventoryBackground.generated.h"

UCLASS()
class DREADNIGHT_API UInventoryBackground : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UCanvasPanel> InventoryCanvas;
	
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UCanvasPanel> OtherInventoryCanvas;
	
public:
	void AddChildToInventoryCanvas(UUserWidget* Widget) const;
	void AddChildToOtherInventoryCanvas(UUserWidget* Widget) const;
	
};
