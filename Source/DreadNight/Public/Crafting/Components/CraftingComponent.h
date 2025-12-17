// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Crafting/Data/Recipe.h"
#include "InventorySystem/InventoryComponent.h"
#include "UI/Widgets/Glossary.h"
#include "CraftingComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DREADNIGHT_API UCraftingComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	TSubclassOf<UGlossary> GetWidget() const { return CraftingWidgetClass; }

protected:

	UCraftingComponent();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	ECraftMethod CraftMethod = ECraftMethod::Basic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Links")
	TSubclassOf<UGlossary> CraftingWidgetClass = nullptr;

private:

	void Craft(FRecipe* Recipe, TObjectPtr<UInventoryComponent> Inventory);

};
