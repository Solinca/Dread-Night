// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Data/ItemDataAsset.h"
#include "Ingredient.generated.h"

USTRUCT(BlueprintType)
struct FIngredient
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemDataAsset> ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 1;

};
