#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Object/ItemInstance.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryUpdatedEventSignature, class UInventoryComponent*, InventoryComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UItemInstance>> Items;
	
	UPROPERTY(BlueprintReadWrite)
	int Size;

public:	
	
	void AddItem(TObjectPtr<UItemInstance> Item);
	void RemoveItemsByType(TObjectPtr<UItemDataAsset> Item);
	void RemoveItemsAt(int SlotIndex, int Amount);
	void UseItemByType(TObjectPtr<UItemDataAsset> Item);
	void UseItemAt(int SlotIndex);
	void DropItems(int SlotIndex, int Amount);
	void Clear();
	
	void TransferItem(TObjectPtr<UInventoryComponent> InventoryComponent, TObjectPtr<UItemInstance> Item, int SlotIndex);
	void SwapItem(TObjectPtr<UInventoryComponent> InventoryComponent, TObjectPtr<UItemInstance> FromItem, TObjectPtr<UItemInstance> ToItem, int SlotIndex);
	
	int GetSize() const { return Size; }
	int GetEmptySlot() const;
	int GetItemSlot(TObjectPtr<UItemDataAsset> Item) const;
	int GetStackableItemSlot(TObjectPtr<UItemDataAsset> Item) const;
	
	bool Contains(TObjectPtr<UItemDataAsset> Item, int StackNumber) const;
	bool IsSlotEmpty(int SlotIndex) const;
	bool IsFull() const;
};
