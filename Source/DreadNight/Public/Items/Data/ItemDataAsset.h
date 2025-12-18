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
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly, meta=(Categories="Item"))
	FGameplayTag Type;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly, meta=(Categories="Rarity"))
	FGameplayTag Rarity;
	
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly, meta=(ClampMin = 1))
	int StackLimit = 1;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly)
	TObjectPtr<UTexture2D> ItemIcon;
};
