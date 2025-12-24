#pragma once

#include "CoreMinimal.h"
#include "InventoryInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "InventorySystem/InventoryComponent.h"
#include "UI/Widgets/InventorySlot.h"
#include "UI/Widgets/InventoryAction.h"
#include "Items/Interface/UsableItem.h"
#include "Inventory.generated.h"

UCLASS()
class DREADNIGHT_API UInventory : public UUserWidget
{
	GENERATED_BODY()

private:
	FVector2D WidgetOffset = FVector2D(0.0f, 0.0f);
	
	inline static TWeakObjectPtr<UInventoryAction> GlobalInventoryAction;

	int CurrentSlotIndex = 0;

protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UWrapBox> InventoryWrapBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventorySlot> InventorySlotClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryAction> InventoryActionClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ItemInfoWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UInventoryComponent> BindInventoryComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UInventoryComponent> BindTargetInventoryComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UInventoryAction> InventoryAction;
	
	UPROPERTY()
	TObjectPtr<UInventoryInfo> InventoryInfoWidget;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor EmptyInventorySlot;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor UsedInventorySlot;

public:
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

	UFUNCTION(BlueprintCallable)
	void OnSelectedHotbarChanged(int SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void BindToInventory(UInventoryComponent* InventoryComponent);
	
	UFUNCTION(BlueprintCallable)
	void BindTargetInventory(UInventoryComponent* InventoryComponent);
	
	UFUNCTION(BlueprintCallable)
	void OnItemActionCreated(int SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void OnItemInfoCreated(int SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void OnItemInfoRemoved();
	
	UFUNCTION(BlueprintCallable)
	void RemoveItemAction();
	
	UWrapBox* GetInventoryWrapBox() const { return InventoryWrapBox; }
	
	UInventoryInfo* GetInventoryInfoWidget() const { return InventoryInfoWidget; }
};
