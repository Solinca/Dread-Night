// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGenerator.generated.h"

class APCGVolume;

UCLASS()
class DREADNIGHT_API AWorldGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldGenerator();

	UPROPERTY(EditAnywhere)
	TArray<APCGVolume*> OnFirstLoadPCG;
	
	UPROPERTY(EditAnywhere)
	TArray<APCGVolume*> AlwaysLoadPCG;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int PCGGenerationNumber;
	int PCGCurrentGenerationNumber;


	void RegisterVolume(APCGVolume* Volume);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateWorld();

	void SetSeed(int Seed);

	
#if WITH_EDITOR

	UFUNCTION(CallInEditor)
	void CleanUp();
	
	UFUNCTION(CallInEditor)
	void Generate();
#endif
	
};
