#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "FoodDataAsset.generated.h"

UCLASS()
class DREADNIGHT_API UFoodDataAsset : public UItemDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1.f))
	float HungerRegenerationValue;
};
