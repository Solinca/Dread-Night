#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Textblock.h"
#include "InventorySystem/InventoryComponent.h"
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
	
	int SlotIndex;
public:
	UFUNCTION(BlueprintCallable)
	virtual void NativePreConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void OnUsePressed();
	UFUNCTION(BlueprintCallable)
	void OnTransferPressed();
	UFUNCTION(BlueprintCallable)
	void OnDropPressed();
	UFUNCTION(BlueprintCallable)
	void OnRemovePressed();
	
	int GetSlotIndex() const { return SlotIndex; }
	void SetSlotIndex(int Index) { SlotIndex = Index; }
	TObjectPtr<UTextBlock> GetUseText() const { return UseText; }
	TObjectPtr<UTextBlock> GetTransferText() const { return TransferText; }
	TObjectPtr<UTextBlock> GetDropText() const { return DropText; }
	TObjectPtr<UTextBlock> GetRemoveText() const { return RemoveText; }
	
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
