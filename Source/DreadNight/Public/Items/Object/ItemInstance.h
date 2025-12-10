#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"

class UItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEmptyItemEventSignature, class UItemInstance*, ItemInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemInstanceStackChangeEventSignature, class UItemInstance*, ItemInstance, int, NewInstanceNumber);


UCLASS()
class DREADNIGHT_API UItemInstance : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UItemDataAsset> ItemData;

	UPROPERTY(BlueprintReadOnly)
	int StackNumber;

	bool TryStealInstance(UItemInstance* Other);

	void DestroyIfEmpty();

	virtual void OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack);
public:
	UItemDataAsset* GetDataAsset() const { return ItemData; };
	int GetStackNumber() const { return StackNumber; };
	
	void SetupItemInstance(UItemDataAsset* DataAsset,const int InitialStack);
	
	enum class EStackMethod
	{
		Fully,
		Partially,
		SameType
	};
 
	bool CanBeStackedWith(const UItemInstance* Other, EStackMethod StackMethod) const;

	bool TryStackWith(UItemInstance* Other);

	bool TryUse(const int NumberOfInstanceToUse);

	bool TryAdd(const int NumberOfInstanceToAdd);

	bool IsEmpty() const;
	
	FEmptyItemEventSignature OnItemDepleted;
	FItemInstanceStackChangeEventSignature OnItemStackChange; 
};
