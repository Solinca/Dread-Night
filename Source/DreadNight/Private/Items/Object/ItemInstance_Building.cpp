#include "Items/Object/ItemInstance_Building.h"
#include "Player/CustomPlayerController.h"

FName UItemInstance_Building::GetActionName()
{
	return FName("Use");
}

void UItemInstance_Building::Use(AActor* Player)
{
	if (APlayerCharacter* Character = Cast<APlayerCharacter>(Player))
	{
		if (ACustomPlayerController* Controller = Cast<ACustomPlayerController>(Character->GetController()))
		{
			if (!Controller->IsPlacingBuilding())
			{
				Controller->CreateBuilding(GetDataAsset());
			}
		}
	}
}

UBuildingDataAsset* UItemInstance_Building::GetDataAsset()
{
	return BuildingDataAsset;
}

void UItemInstance_Building::OnSetupItemInstance(UItemDataAsset* DataAsset, const int InitialStack)
{
	Super::OnSetupItemInstance(DataAsset, InitialStack);
	
	if (BuildingDataAsset = Cast<UBuildingDataAsset>(DataAsset); !BuildingDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("DataAsset %s is not the expected type !"), *DataAsset->GetName());
	}
}
