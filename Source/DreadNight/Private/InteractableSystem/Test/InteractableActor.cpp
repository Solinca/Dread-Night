// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableSystem/Test/InteractableActor.h"

#include "InteractableSystem/Components/InteractableComponent.h"


AInteractableActor::AInteractableActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));
}


void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableActor::OnInteract_Implementation(AActor* Caller)
{
	InteractableComponent->Interact(Caller);
}

void AInteractableActor::OnFocusGained_Implementation()
{
	MeshComponent->SetOverlayMaterial(InteractableComponent->GetOutlineMaterial(true));
}

void AInteractableActor::OnFocusLost_Implementation()
{
	MeshComponent->SetOverlayMaterial(InteractableComponent->GetOutlineMaterial(false));
}

FText AInteractableActor::GetInteractionPromptText_Implementation()
{
	return InteractableComponent->GetInteractionPromptText();
}