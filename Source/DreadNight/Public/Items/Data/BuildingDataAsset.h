#pragma once

#include "CoreMinimal.h"
#include "Items/Data/ItemDataAsset.h"
#include "Actors/Building.h"
#include "BuildingDataAsset.generated.h"

UCLASS()
class DREADNIGHT_API UBuildingDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<ABuilding> BuildingClass;
};
