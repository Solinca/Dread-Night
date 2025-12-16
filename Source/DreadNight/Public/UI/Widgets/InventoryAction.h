#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Textblock.h"
#include "InventorySystem/InventoryComponent.h"
#include "UI/Widgets/InventorySlider.h"
#include "InventoryAction.generated.h"

UCLASS()
class DREADNIGHT_API UInventoryAction : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> UseButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> TransferButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> DropButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> RemoveButton;
	
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> UseText;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> TransferText;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> DropText;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> RemoveText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventorySlider> InventorySliderWidgetClass;
	UPROPERTY()
	TObjectPtr<UInventorySlider> InventorySlider;
	
	int SlotIndex;
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetupAction(UInventoryComponent* OwningInventory, UInventoryComponent* TargetInventory, int Index);
	
	UFUNCTION(BlueprintCallable)
	void OnUsePressed();
	UFUNCTION(BlueprintCallable)
	void OnTransferPressed();
	UFUNCTION(BlueprintCallable)
	void OnDropPressed();
	UFUNCTION(BlueprintCallable)
	void OnRemovePressed();
	UFUNCTION()
	void OnDropAmountSelected(int Amount);
	UFUNCTION()
	void OnRemoveAmountSelected(int Amount);
	
	int GetSlotIndex() const { return SlotIndex; }
	void SetSlotIndex(int Index) { SlotIndex = Index; }
	TObjectPtr<UButton> GetUseButton() const { return UseButton; }
	TObjectPtr<UButton> GetTransferButton() const { return TransferButton; }
	TObjectPtr<UButton> GetDropButton() const { return DropButton; }
	TObjectKey<UButton> GetRemoveButton() const { return RemoveButton; }
	TObjectPtr<UTextBlock> GetUseText() const { return UseText; }
	TObjectPtr<UTextBlock> GetTransferText() const { return TransferText; }
	TObjectPtr<UTextBlock> GetDropText() const { return DropText; }
	TObjectPtr<UTextBlock> GetRemoveText() const { return RemoveText; }
	
	TObjectPtr<UInventoryComponent> InventoryComponent;
	TObjectPtr<UInventoryComponent> TargetInventoryComponent;
};
