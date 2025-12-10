#include "Items/Object/ItemInstance_Drink.h"

#include "Items/Data/DrinkDataAsset.h"

FName UItemInstance_Drink::GetActionName()
{
	return FName(TEXT("Drink"));
}

void UItemInstance_Drink::Use(AActor* Player)
{
	StackNumber--;
	OnItemStackChange.Broadcast(this, StackNumber);
	DestroyIfEmpty();
}

void UItemInstance_Drink::OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
	if (DrinkDataAsset = Cast<UDrinkDataAsset>(DataAsset); !DrinkDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("DataAsset %s is not the expected type !"), *DataAsset->GetName());
	}
}
