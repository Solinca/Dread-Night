// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Object/ItemInstance.h"

#include "Items/Data/ItemDataAsset.h"

bool UItemInstance::TryStealInstance(UItemInstance* Other)
{
	if (!Other || !ItemData)
		return false;

	if (CanBeStackedWith(Other, EStackMethod::Fully))
	{
		TryAdd(Other->StackNumber);
		Other->TryUse(Other->StackNumber);		 
		return true;
	}
	if (CanBeStackedWith(Other, EStackMethod::Partially))
	{
		const int InstanceToAdd = StackNumber - ItemData->StackLimit;		
		TryAdd(InstanceToAdd);
		Other->TryUse(InstanceToAdd);	
		return true;		
	}

	return false;

}

void UItemInstance::DestroyIfEmpty()
{
	if (StackNumber <= 0)
	{
		OnItemDepleted.Broadcast(this);
		
		MarkAsGarbage();
	}
}

void UItemInstance::SetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
	ItemData = DataAsset;
	StackNumber = InitialStack;
}
 

bool UItemInstance::CanBeStackedWith(const UItemInstance* Other, EStackMethod StackMethod) const
{
	if (!Other || !ItemData)
		return false;

	const bool bIsSameType = Other->ItemData->Type.MatchesTagExact(ItemData->Type);
	
	switch (StackMethod) {
	case EStackMethod::Fully: 
		return bIsSameType && StackNumber + Other->StackNumber <= ItemData->StackLimit; 
	case EStackMethod::Partially: 
		return bIsSameType && StackNumber < ItemData->StackLimit; 
	case EStackMethod::SameType:
		return bIsSameType; 
	}
	
 
	return false;
}

 

bool UItemInstance::TryStackWith(UItemInstance* Other)
{
	if (!Other || !ItemData)
		return false;

	return TryStealInstance(Other);
}

bool UItemInstance::TryAdd(const int NumberOfInstanceToAdd)
{
	if (StackNumber + NumberOfInstanceToAdd <= ItemData->StackLimit)
		return false;

	StackNumber += NumberOfInstanceToAdd;
	OnItemStackChange.Broadcast(this, StackNumber); 
	return true;
}

bool UItemInstance::TryUse(const int NumberOfInstanceToUse)
{
	if (NumberOfInstanceToUse > StackNumber)
		return false;

	StackNumber -= NumberOfInstanceToUse;
	OnItemStackChange.Broadcast(this, StackNumber);
	DestroyIfEmpty();
	return true;
}
