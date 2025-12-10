// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/Actors/CraftingActor.h"



void ACraftingActor::OnInteract_Implementation(AActor* Caller)
{

	// Each Actor have to define it, here I just destroy it as a placeholder
	Destroy();

}



void ACraftingActor::OnFocusGained_Implementation()
{

	MeshComponent->SetOverlayMaterial(InteractableComponent->GetOutlineMaterial());

}



void ACraftingActor::OnFocusLost_Implementation()
{

	MeshComponent->SetOverlayMaterial(nullptr);

}



FText ACraftingActor::GetInteractionPromptText_Implementation()
{

	return InteractableComponent->GetInteractionPromptText();

}



ACraftingActor::ACraftingActor()
{

	PrimaryActorTick.bCanEverTick = true;

}



void ACraftingActor::BeginPlay()
{

	Super::BeginPlay();

}
