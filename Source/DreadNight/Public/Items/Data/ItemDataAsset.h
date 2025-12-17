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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="Item"))
	FGameplayTag Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="Rarity"))
	FGameplayTag Rarity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin = 1))
	int StackLimit = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> ItemIcon;
};
