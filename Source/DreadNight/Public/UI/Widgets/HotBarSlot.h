#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/InventorySlot.h"
#include "HotBarSlot.generated.h"

class UImage;
class UButton;
class UTextBlock;
class UItemInstance;

UCLASS()
class DREADNIGHT_API UHotBarSlot : public UInventorySlot
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ShortcutText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectedSlot;
	
	UPROPERTY()
	TObjectPtr<UItemInstance> CurrentItemInstance;

	int32 CurrentQuantity;

public:
	UFUNCTION(BlueprintCallable, Category = "HotBarSlot")
	void UpdateSlot(UItemInstance* InItemInstance, const int32 InQuantity);
	
	UFUNCTION(BlueprintCallable, Category = "HotBarSlot")
	void SetShortcutText(const FText& InText);

	virtual void SetIsSelected(bool IsSelected) override;
	
	virtual void SetStackText(int Stack) override;
	virtual void Reset(const FSlateBrush& Brush) override;

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
