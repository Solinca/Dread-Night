
#include "UI/Glossary/CraftingFunctionLibrary.h"

#include "UI/Glossary/CraftingRecipeItem.h"

TArray<UCraftingRecipeItem*> UCraftingFunctionLibrary::ConvertDataTableToRecipeItems(UDataTable* DataTable)
{
	TArray<UCraftingRecipeItem*> ResultItems;

	if (!DataTable)
	{
		return ResultItems;
	}
	
	static const FString ContextString(TEXT("CraftingContext"));

	for (const FName& RowName : DataTable->GetRowNames())
	{
		if (const FCraftingRecipe* RowData = DataTable->FindRow<FCraftingRecipe>(RowName, ContextString))
		{
			UCraftingRecipeItem* NewItem = NewObject<UCraftingRecipeItem>();
            
			NewItem->RecipeData = *RowData;
			NewItem->RowName = RowName;
			

			ResultItems.Add(NewItem);
		}
	}

	return ResultItems;
}
