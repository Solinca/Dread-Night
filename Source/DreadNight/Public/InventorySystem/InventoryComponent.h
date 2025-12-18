#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Object/ItemInstance.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAddedEventSignature, class UItemInstance*, ItemInstance, int, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemovedEventSignature, int, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemClearedEventSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemModifiedEventSignature, class UItemInstance*, ItemInstance, int, ItemSlot);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(SaveGame)
	TArray<FItemInstanceSave> SavedItems;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UItemInstance>> Items;
	
	UPROPERTY(EditAnywhere, SaveGame,BlueprintReadWrite)
	int Size = 20;
	
public:	
	
	UFUNCTION(BlueprintCallable)
	void AddItem(UItemInstance* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveItemsByType(UItemDataAsset* Item, int Amount);
	void RemoveItemsAt(int SlotIndex, int Amount);
	void UseItemByType(UItemDataAsset* Item);
	void UseItemAt(int SlotIndex);
	void Clear();
	
	void TransferItem(UInventoryComponent* TargetInventory, UItemInstance* Item, int SlotIndex);
	void SwapItem(UInventoryComponent* TargetInventory, UItemInstance* FromItem, UItemInstance* ToItem, int SlotIndex);
	
	int GetSize() const { return Size; }
	void SetSize(int NewSize) { Size = NewSize; }
	TOptional<int> GetEmptySlot() const;
	UItemInstance* GetItemAtSlot(int SlotIndex) const;
	UItemDataAsset* GetItemTypeAtSlot(int SlotIndex) const;
	TOptional<int> GetItemSlot(UItemDataAsset* Item) const;
	TOptional<int> GetItemInstanceSlot(UItemInstance* Item) const;
	TOptional<int> GetStackableItemSlot(UItemDataAsset* Item) const;
	
	UFUNCTION(BlueprintCallable)
	bool Contains(UItemDataAsset* Item, int StackNumber) const;
	bool IsSlotEmpty(int SlotIndex) const;
	bool IsFull() const;
	int GetInventoryLimitSize() const;

	void SerializeInventory();
	void DeserializeInventory();
	
	FOnItemAddedEventSignature OnItemAdded;
	FOnItemRemovedEventSignature OnItemRemoved;
	FOnItemModifiedEventSignature OnItemModified;
	FOnItemClearedEventSignature OnItemCleared;
};
