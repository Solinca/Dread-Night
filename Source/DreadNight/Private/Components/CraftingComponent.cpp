// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CraftingComponent.h"



void UCraftingComponent::Craft(FRecipe* Recipe, TObjectPtr<UInventory> Inventory)
{

	for (FIngredient Ingredient : Recipe->Ingredients)
	{

		/*
		if(!Inventory->Contains(Ingredient.ItemType, Ingredient.Quantity)) break;
		*/

	}

	for (FIngredient Ingredient : Recipe->Ingredients)
	{

		//Inventory->Remove(Ingredient.ItemType, Ingredient.Quantity);

	}

	//Inventory->Add(Recipe->TargetItem, 1);

}



void UCraftingComponent::OpenUI()
{

	//CraftingWidget = CreateWidget<UCraftingWidget>(this, CraftingWidgetClass);

	//CraftingWidget->AddToViewport();

}



UCraftingComponent::UCraftingComponent()
{



}



void UCraftingComponent::BeginPlay()
{

	Super::BeginPlay();
	
}

