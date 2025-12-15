#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "InventorySystem/InventoryComponent.h"
#include "InventorySlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemActionCreatedEventSignature, int, SlotIndex);

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
	
	TObjectPtr<UInventoryComponent> BindInventoryComponent;
	TObjectPtr<UInventoryComponent> BindTargetInventoryComponent;
	
	int SlotIndex;
	
	bool HasRightClicked = false;
public:
	UFUNCTION(BlueprintCallable)
	virtual void NativePreConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION(BlueprintCallable)
	void SetItemImage(UTexture2D* Texture);
	UFUNCTION(BlueprintCallable)
	void SetStackText(int Stack);
	
	UFUNCTION(BlueprintCallable)
	int GetSlotIndex() const { return SlotIndex; }
	UFUNCTION(BlueprintCallable)
	void SetSlotIndex(int Index) { SlotIndex = Index; }

	UFUNCTION(BlueprintCallable)
	void SetupSlot(UInventoryComponent* OwningInventory, UInventoryComponent* TargetInventory, int Index);
	UFUNCTION(BlueprintCallable)
	void SlotAction();
	UFUNCTION(BlueprintCallable)
	void BindToInventory(UInventoryComponent* InventoryComponent);
	
	UFUNCTION(BlueprintCallable)
	void Reset(const FSlateBrush& Brush);
	
	const FSlateBrush& GetImageBrush() const;
	
	FOnItemActionCreatedEventSignature OnItemActionCreated;
};
