#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryQuickAddButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickAddButtonClickedEventSignature, int, SlotIndex);

UCLASS()
class DREADNIGHT_API UInventoryQuickAddButton : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> QuickAddButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> QuickAddText;
	
	int SlotIndex = 1;
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void SetupButtonSlot(int Index);
	UFUNCTION()
	void OnButtonClicked();
	
	UButton* GetQuickAddButton() const { return QuickAddButton; }
	UTextBlock* GetQuickAddText() const { return QuickAddText; }
	
	FOnQuickAddButtonClickedEventSignature OnQuickAddButtonClicked;
};
