// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemInstance.generated.h"

class UItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEmptyItemEvent,class UItemInstance*, ItemInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemInstanceStackChangeEvent,class UItemInstance*, ItemInstance, int, NewInstanceNumber);
/**
 * 
 */
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
public:
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
	
	FEmptyItemEvent OnItemDepleted;
	FItemInstanceStackChangeEvent OnItemStackChange; 
};
