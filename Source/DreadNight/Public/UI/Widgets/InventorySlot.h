#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "InventorySystem/InventoryComponent.h"
#include "InventorySlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemActionCreatedEventSignature, int, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInfoCreatedEventSignature, int, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemInfoRemovedEventSignature);

UCLASS()
class DREADNIGHT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> StackText;

	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> ItemButton;

	UPROPERTY(Transient)
	TObjectPtr<UInventoryComponent> BindInventoryComponent;
	UPROPERTY(Transient)
	TObjectPtr<UInventoryComponent> BindTargetInventoryComponent;
	
	int SlotIndex;
	
	bool HasRightClicked = false;
	bool IsMouseOver = false;
public:
	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION(BlueprintCallable)
	void SetItemImage(UTexture2D* Texture);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetStackText(int Stack);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetImageColor(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable)
	virtual void SetIsSelected(bool IsSelected) {};
	
	UFUNCTION(BlueprintCallable)
	int GetSlotIndex() const { return SlotIndex; }
	
	UFUNCTION(BlueprintCallable)
	void SetSlotIndex(int Index) { SlotIndex = Index; }

	UFUNCTION(BlueprintCallable)
	void SetupSlot(UInventoryComponent* OwningInventory, UInventoryComponent* TargetInventory, int Index);
	
	UFUNCTION(BlueprintCallable)
	void SlotAction();
	
	UFUNCTION(BlueprintCallable)
	void InfoAction();
	
	UFUNCTION(BlueprintCallable)
	void BindToInventory(UInventoryComponent* InventoryComponent);
	
	UFUNCTION(BlueprintCallable)
	virtual void Reset(const FSlateBrush& Brush);
	
	const FSlateBrush& GetImageBrush() const;
	
	FOnItemActionCreatedEventSignature OnItemActionCreated;
	FOnItemInfoCreatedEventSignature OnItemInfoCreated;
	FOnItemInfoRemovedEventSignature OnItemInfoRemoved;
};
