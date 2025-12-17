// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/Components/CraftingComponent.h"
#include "Items/Helper/ItemInstanceFactory.h"



UCraftingComponent::UCraftingComponent()
{



}



void UCraftingComponent::BeginPlay()
{

	Super::BeginPlay();

}



void UCraftingComponent::Craft(FRecipe* Recipe, TObjectPtr<UInventoryComponent> Inventory)
{

	for (FIngredient Ingredient : Recipe->Ingredients)
	{

		if (!Inventory->Contains(Ingredient.ItemType, Ingredient.Quantity)) break;

	}

	for (FIngredient Ingredient : Recipe->Ingredients)
	{

		Inventory->RemoveItemsByType(Ingredient.ItemType, Ingredient.Quantity);

	}

	Inventory->AddItem(UItemInstanceFactory::CreateItem(this, Recipe->TargetItem, 1));

}