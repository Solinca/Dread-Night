// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "InventorySystem/InventoryComponent.h"
#include "UI/Widgets/InventorySlot.h"
#include "Inventory.generated.h"

UCLASS()
class DREADNIGHT_API UInventory : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UWrapBox> InventoryWrapBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventorySlot> InventorySlot;
public:
	UFUNCTION(BlueprintCallable)
	virtual void NativePreConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetSize(int Size);
	UFUNCTION(BlueprintCallable)
	void OnItemAdded(UItemInstance* Item, int SlotIndex);
	UFUNCTION(BlueprintCallable)
	void OnItemRemoved(int SlotIndex);
	UFUNCTION(BlueprintCallable)
	void OnItemModified(UItemInstance* Item, int SlotIndex);
	UFUNCTION(BlueprintCallable)
	void OnItemsCleared();
	
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
