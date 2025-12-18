#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryQuickAddButton.generated.h"

UCLASS()
class DREADNIGHT_API UInventoryQuickAddButton : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> QuickAddButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> QuickAddText;
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	TObjectPtr<UButton> GetQuickAddButton() const { return QuickAddButton; }
	void SetQuickAddText(const FText& Text) { QuickAddText->SetText(Text); }
	
};
