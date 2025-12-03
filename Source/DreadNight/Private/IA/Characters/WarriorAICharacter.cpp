// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Characters/WarriorAICharacter.h"


// Sets default values
AWarriorAICharacter::AWarriorAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWarriorAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
