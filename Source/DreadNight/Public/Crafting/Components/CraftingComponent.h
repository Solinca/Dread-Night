// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Crafting/Data/Recipe.h"
#include "CraftingComponent.generated.h"

class UInventoryComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DREADNIGHT_API UCraftingComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	void Craft(FRecipe* Recipe, TObjectPtr<UInventoryComponent> Inventory);

	void OpenGUI();

protected:

	UCraftingComponent();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	ECraftMethod CraftMethod;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Links")
	//TSubclassOf<UCraftingWidget> CraftingWidgetClass = nullptr;

private:

	//TObjectPtr<UCraftingWidget> CraftingWidget;

};
