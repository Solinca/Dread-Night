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
	// Each Actor have to define it, here I just destroy it as a placeholder
	Destroy();
}

void AInteractableActor::OnFocusGained_Implementation()
{
	MeshComponent->SetOverlayMaterial(InteractableComponent->GetOutlineMaterial());
}

void AInteractableActor::OnFocusLost_Implementation()
{
	MeshComponent->SetOverlayMaterial(nullptr);
}

FText AInteractableActor::GetInteractionPromptText_Implementation()
{
	return InteractableComponent->GetInteractionPromptText();
}