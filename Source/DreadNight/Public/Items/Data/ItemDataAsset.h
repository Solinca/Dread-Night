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
	
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly, meta=(ClampMin = 1))
	int StackLimit = 1;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly)
	TObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditAnywhere)
	FVector ItemScale = FVector::One();
};
