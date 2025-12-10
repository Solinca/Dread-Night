
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CraftingFunctionLibrary.generated.h"



UCLASS()
class DREADNIGHT_API UCraftingFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static TArray<class UCraftingRecipeItem*> ConvertDataTableToRecipeItems(UDataTable* DataTable);
};
