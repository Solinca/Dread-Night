#pragma once

#include "CoreMinimal.h"
#include "Items/Data/ItemDataAsset.h"
#include "LootData.generated.h"

USTRUCT(BlueprintType)
struct DREADNIGHT_API FLootData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemDataAsset> ItemDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Percentage = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinDroppedAmount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxDroppedAmount = 1;
	
	
};
