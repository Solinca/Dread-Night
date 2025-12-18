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

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GameInstance)
		return;

	if (!GameInstance->IsNewGame())
	{
		GameInstance->Load(GetWorld());
	}

	 
	
	for (auto Element : AlwaysLoadPCG)
	{
		RegisterVolume(Element);
	}
	
	if (GameInstance->IsNewGame())
	{		
		for (auto Element : OnFirstLoadPCG)
		{
			RegisterVolume(Element);
		}
	}
	
	SetSeed(GameInstance->GetSeed());
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
	CleanUp();
	Generate();
}

void AWorldGenerator::SetSeed(int Seed)
{
	DoForAllVolume([Seed](APCGVolume* Volume)
	{
		Volume->PCGComponent->Seed = Seed;
	});
}
 
void AWorldGenerator::CleanUp()
{
	DoForAllVolume([](APCGVolume* Volume)
	{
		Volume->PCGComponent->Cleanup();
	});
}

void AWorldGenerator::GenerateInEditor()
{
	DoForAllVolume([](APCGVolume* Volume)
	{
		Volume->PCGComponent->Generate();
	});	
}

void AWorldGenerator::Generate()
{
	for (auto Element : AlwaysLoadPCG)
	{
		Element->PCGComponent->Generate();
	}

	if (GameInstance->IsNewGame())
	{
		for (auto Element : OnFirstLoadPCG)
		{
			Element->PCGComponent->Generate();
		}
	}
}
 