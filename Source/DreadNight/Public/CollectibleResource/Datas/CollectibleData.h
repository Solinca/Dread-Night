#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CollectibleData.generated.h"

class ACollectibleResource;

USTRUCT(BlueprintType)
struct FCollectibleData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UStaticMesh>> StaticMeshArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACollectibleResource> CollectibleResource;
};