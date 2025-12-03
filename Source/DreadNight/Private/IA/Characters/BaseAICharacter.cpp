// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Characters/BaseAICharacter.h"


// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
