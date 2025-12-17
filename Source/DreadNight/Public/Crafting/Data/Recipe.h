// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftMethod.h"
#include "Ingredient.h"
#include "Recipe.generated.h"



USTRUCT(BlueprintType)
struct FRecipe : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemDataAsset> TargetItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TargetQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECraftMethod CraftMethod = ECraftMethod::Basic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIngredient> Ingredients;

};

UCLASS(BlueprintType)
class DREADNIGHT_API URecipeItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (ExposeOnSpawn = true))
	FRecipe RecipeData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (ExposeOnSpawn = true))
	FName RowName;
};