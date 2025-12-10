#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Object/ItemInstance.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemAddedEventSignature, class UItemInstance*, InventoryComponent, int, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemovedEventSignature, int, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemClearedEventSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemModifiedEventSignature, class UItemInstance*, InventoryComponent, int, ItemSlot);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UItemInstance>> Items;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Size = 20;

public:	
	
	void AddItem(UItemInstance* Item);
	void RemoveItemsByType(UItemDataAsset* Item);
	void RemoveItemsAt(int SlotIndex, int Amount);
	void UseItemByType(UItemDataAsset* Item);
	void UseItemAt(int SlotIndex);
	void DropItems(int SlotIndex, int Amount);
	void Clear();
	
	void TransferItem(UInventoryComponent* InventoryComponent, UItemInstance* Item, int SlotIndex);
	void SwapItem(UInventoryComponent* InventoryComponent, UItemInstance* FromItem, UItemInstance* ToItem, int SlotIndex);
	
	int GetSize() const { return Size; }
	int GetEmptySlot() const;
	UItemInstance* GetItemAtSlot(int SlotIndex) const;
	UItemDataAsset* GetItemTypeAtSlot(int SlotIndex) const;
	int GetItemSlot(UItemDataAsset* Item) const;
	int GetStackableItemSlot(UItemDataAsset* Item) const;
	
	bool Contains(UItemDataAsset* Item, int StackNumber) const;
	bool IsSlotEmpty(int SlotIndex) const;
	bool IsFull() const;
	
	FItemAddedEventSignature OnItemAdded;
	FItemRemovedEventSignature OnItemRemoved;
	FItemModifiedEventSignature OnItemModified;
	FItemClearedEventSignature OnItemCleared;
};
