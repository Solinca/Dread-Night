// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crafting/Components/CraftingComponent.h"
#include "InteractableSystem/Components/InteractableComponent.h"
#include "InteractableSystem/Interfaces/Interactable.h"
#include "CraftingActor.generated.h"

UCLASS()
class DREADNIGHT_API ACraftingActor : public AActor, public IInteractable
{

	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInteractableComponent> InteractableComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCraftingComponent> CraftingComponent;

public:

	// Begin IInteractable Interface
	virtual void OnInteract_Implementation(AActor* Caller) override;
	virtual void OnFocusGained_Implementation() override;
	virtual void OnFocusLost_Implementation() override;
	virtual FText GetInteractionPromptText_Implementation() override;
	// End IInteractable Interface

protected:

	ACraftingActor();

	virtual void BeginPlay() override;

};
