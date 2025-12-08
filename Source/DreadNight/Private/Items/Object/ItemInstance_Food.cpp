#include "Items/Object/ItemInstance_Food.h"

#include "Items/Data/FoodDataAsset.h"

FName UItemInstance_Food::GetActionName()
{
	return FName(TEXT("Eat"));
}

void UItemInstance_Food::Use(AActor* Player)
{
}

void UItemInstance_Food::OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
	if (FoodDataAsset = Cast<UFoodDataAsset>(DataAsset); !FoodDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("DataAsset %s is not the expected type !"), *DataAsset->GetName());
	}
}
