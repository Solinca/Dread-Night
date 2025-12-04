#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h" 
#include "ItemDataAsset.generated.h"


UCLASS()
class DREADNIGHT_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta=(Categories="Item"))
	FGameplayTag Type;

	UPROPERTY(EditAnywhere, meta=(Categories="Rarity"))
	FGameplayTag Rarity;
	
	UPROPERTY(EditAnywhere, meta=(ClampMin = 1))
	int StackLimit = 1;

	UPROPERTY(EditAnywhere)
	FName ItemName;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ItemIcon;
};
