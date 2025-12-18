#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"

class UItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEmptyItemEventSignature, class UItemInstance*, ItemInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemInstanceStackChangeEventSignature, class UItemInstance*, ItemInstance, int, NewInstanceNumber);



UCLASS()
class DREADNIGHT_API UItemInstance : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, SaveGame)
	TObjectPtr<UItemDataAsset> ItemData;

	UPROPERTY(BlueprintReadOnly, SaveGame)
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

	bool TryUse(AActor* User);
	
	bool TryRemove(const int NumberOfInstanceToRemove);

	bool TryAdd(const int NumberOfInstanceToAdd);

	bool IsEmpty() const;
	
	FOnEmptyItemEventSignature OnItemDepleted;
	FOnItemInstanceStackChangeEventSignature OnItemStackChange; 
};

USTRUCT(NotBlueprintType)
struct FItemInstanceSave
{
	GENERATED_BODY()
	UPROPERTY(SaveGame)
	TSoftObjectPtr<UItemDataAsset> Type;
	UPROPERTY(SaveGame)
	int StackNumber = 0;

	static FItemInstanceSave SerializeItemInstance(UItemInstance* Item);

	UItemInstance* DeserializeItemInstance(UObject* Context) const;
};