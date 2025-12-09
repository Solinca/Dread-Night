
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CraftingFunctionLibrary.generated.h"



UCLASS()
class DREADNIGHT_API UCraftingFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Crafting System")
	static TArray<class UCraftingRecipeItem*> ConvertDataTableToRecipeItems(UDataTable* DataTable);
};
