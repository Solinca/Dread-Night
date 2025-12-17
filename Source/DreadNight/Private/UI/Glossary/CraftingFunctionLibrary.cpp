
#include "UI/Glossary/CraftingFunctionLibrary.h"

#include "Crafting/Data/Recipe.h"

TArray<URecipeItem*> UCraftingFunctionLibrary::ConvertDataTableToRecipeItems(UDataTable* DataTable)
{
	TArray<URecipeItem*> ResultItems;

	if (!DataTable)
	{
		return ResultItems;
	}
	
	static const FString ContextString(TEXT("CraftingContext"));

	for (const FName& RowName : DataTable->GetRowNames())
	{
		if (const FRecipe* RowData = DataTable->FindRow<FRecipe>(RowName, ContextString))
		{
			URecipeItem* NewItem = NewObject<URecipeItem>();
            
			NewItem->RecipeData = *RowData;
			NewItem->RowName = RowName;
			

			ResultItems.Add(NewItem);
		}
	}

	return ResultItems;
}
