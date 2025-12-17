// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/Actors/CraftingActor.h"



void ACraftingActor::OnInteract_Implementation(AActor* Caller)
{

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

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));

	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(TEXT("Crafting Component"));

}



void ACraftingActor::BeginPlay()
{

	Super::BeginPlay();

}
