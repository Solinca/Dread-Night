#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "ArmorDataAsset.generated.h"

 
UCLASS()
class DREADNIGHT_API UArmorDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	float DamageReductionMultiplier;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> ArmorMesh;
	UPROPERTY(EditAnywhere)
	bool bIsHelmet;
};
