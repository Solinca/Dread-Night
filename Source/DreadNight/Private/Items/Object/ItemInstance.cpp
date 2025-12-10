#include "Items/Object/ItemInstance.h"
#include "Items/Interface/UsableItem.h"
#include "Items/Data/ItemDataAsset.h"

bool UItemInstance::TryStealInstance(UItemInstance* Other)
{
	if (!Other || !ItemData)
		return false;

	if (CanBeStackedWith(Other, EStackMethod::Fully))
	{
		TryAdd(Other->StackNumber);
		Other->TryRemove(Other->StackNumber);		 
		return true;
	}
	if (CanBeStackedWith(Other, EStackMethod::Partially))
	{
		const int InstanceToAdd = StackNumber - ItemData->StackLimit;		
		TryAdd(InstanceToAdd);
		Other->TryRemove(InstanceToAdd);	
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

void UItemInstance::OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
}

void UItemInstance::SetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
	ItemData = DataAsset;
	StackNumber = InitialStack;
	OnSetupItemInstance(DataAsset,InitialStack);
}
 

bool UItemInstance::CanBeStackedWith(const UItemInstance* Other, EStackMethod StackMethod) const
{
	if (!Other || !ItemData)
		return false;

	const bool bIsSameType = Other->ItemData->Type.MatchesTagExact(ItemData->Type);

	if (!bIsSameType)
		return false;
	
	switch (StackMethod) {
	case EStackMethod::Fully: 
		return StackNumber + Other->StackNumber <= ItemData->StackLimit; 
	case EStackMethod::Partially: 
		return StackNumber < ItemData->StackLimit; 
	case EStackMethod::SameType:
		return true; 
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
	if (StackNumber + NumberOfInstanceToAdd > ItemData->StackLimit)
		return false;

	StackNumber += NumberOfInstanceToAdd;
	OnItemStackChange.Broadcast(this, StackNumber); 
	return true;
}

bool UItemInstance::IsEmpty() const
{
	return StackNumber == 0;
}

TObjectPtr<UItemDataAsset> UItemInstance::GetItemDataAsset() const
{
	return ItemData;
}

int UItemInstance::GetStackNumber() const
{
	return StackNumber;
}

bool UItemInstance::TryUse(const int NumberOfInstanceToUse,AActor* User)
{
	if (NumberOfInstanceToUse > StackNumber)
		return false;

	if (IUsableItem* UsableItem = Cast<IUsableItem>(this))
	{
		UsableItem->Use(User);
	}
	
	return true;
}

bool UItemInstance::TryRemove(const int NumberOfInstanceToRemove)
{
	if (NumberOfInstanceToRemove > StackNumber)
		return false;

	StackNumber -= NumberOfInstanceToRemove;
	OnItemStackChange.Broadcast(this, StackNumber);
	DestroyIfEmpty();
	return true;
}
