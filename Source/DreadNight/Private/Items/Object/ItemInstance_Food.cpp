#include "Items/Object/ItemInstance_Food.h"

#include "Components/ConditionStateComponent.h"
#include "Items/Data/FoodDataAsset.h"

FName UItemInstance_Food::GetActionName()
{
	return FName(TEXT("Eat"));
}

void UItemInstance_Food::Use(AActor* Player)
{
	if (IsEmpty())
		return;
	
	UConditionStateComponent* StateComponent = Player->GetComponentByClass<UConditionStateComponent>();
	if (!StateComponent)
		return;

	StateComponent->AddHungerValue(FoodDataAsset->HungerRegenerationValue);
	StackNumber--;
	OnItemStackChange.Broadcast(this, StackNumber);
	DestroyIfEmpty();
}

void UItemInstance_Food::OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
	if (FoodDataAsset = Cast<UFoodDataAsset>(DataAsset); !FoodDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("DataAsset %s is not the expected type !"), *DataAsset->GetName());
	}
}
