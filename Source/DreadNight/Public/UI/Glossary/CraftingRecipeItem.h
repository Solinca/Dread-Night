
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CraftingRecipeItem.generated.h"

USTRUCT(BlueprintType)
struct FCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	FText Ingredients; 
};


UCLASS(BlueprintType)
class DREADNIGHT_API UCraftingRecipeItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (ExposeOnSpawn = true))
	FCraftingRecipe RecipeData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (ExposeOnSpawn = true))
	FName RowName;
};
