// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Crafting/Data/Recipe.h"
#include "InventorySystem/InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "CraftingComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DREADNIGHT_API UCraftingComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	void OnCraft(FRecipe* Recipe);

	void OpenGUI(AActor* Caller);

protected:

	UCraftingComponent();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	ECraftMethod CraftMethod;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Links")
	//TSubclassOf<UCraftingWidget> CraftingWidgetClass = nullptr;

private:

	void Craft(FRecipe* Recipe, TObjectPtr<UInventoryComponent> Inventory);

	AActor* User;

	//TObjectPtr<UCraftingWidget> CraftingWidget;

};
