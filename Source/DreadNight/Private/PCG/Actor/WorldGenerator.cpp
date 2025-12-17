// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/Actor/WorldGenerator.h"

#include "Global/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PCG/Public/PCGVolume.h"
#include "PCG/Public/PCGComponent.h"

// Sets default values
AWorldGenerator::AWorldGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (auto Element : PCG_Array)
	{
		RegisterVolume(Element);
	}
	GenerateWorld();
}

void AWorldGenerator::RegisterVolume(APCGVolume* Volume)
{
	if (!Volume)
		return;
	Volume->PCGComponent->OnPCGGraphGeneratedDelegate.AddLambda([this](UPCGComponent*)
	{
		++PCGCurrentGenerationNumber;
		if (PCGCurrentGenerationNumber >= PCGGenerationNumber)
		{
			if(UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				GameInstance->OnPCGEndGeneration.Broadcast();				
			}
		}
	});
	++PCGGenerationNumber;
}

// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldGenerator::GenerateWorld()
{
	int Seed = FMath::RandRange(0, 999999);
	for (APCGVolume* Volume : PCG_Array)
	{
		Volume->PCGComponent->Cleanup();
		Volume->PCGComponent->Seed = Seed;
		Volume->PCGComponent->Generate();
	}
}

#if WITH_EDITOR
void AWorldGenerator::CleanUp()
{
	for (auto Element : PCG_Array)
	{
		Element->PCGComponent->Cleanup();
	}
}

void AWorldGenerator::Generate()
{
	for (auto Element : PCG_Array)
	{
		Element->PCGComponent->Generate();
	}
}

#endif
