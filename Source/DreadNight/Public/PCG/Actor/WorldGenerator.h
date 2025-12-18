// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGenerator.generated.h"

class UMyGameInstance;
class APCGVolume;

UCLASS()
class DREADNIGHT_API AWorldGenerator : public AActor
{
	GENERATED_BODY()

protected:
	// Sets default values for this actor's properties
	AWorldGenerator();

	UPROPERTY(EditAnywhere)
	TArray<APCGVolume*> OnFirstLoadPCG;
	
	UPROPERTY(EditAnywhere)
	TArray<APCGVolume*> AlwaysLoadPCG;
	
	virtual void BeginPlay() override;

	int PCGGenerationNumber;
	int PCGCurrentGenerationNumber; 

	UPROPERTY(Transient)
	UMyGameInstance* GameInstance;


	void RegisterVolume(APCGVolume* Volume);

	template<typename Function>
	void DoForAllVolume(const Function& Action)
	{
		for (APCGVolume* Volume : OnFirstLoadPCG)
		{
			Action(Volume);
		}
		for (APCGVolume* Volume : AlwaysLoadPCG)
		{
			Action(Volume);
		}
	}
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateWorld();

	void SetSeed(int Seed);
	
	void Generate();
	
	UFUNCTION(CallInEditor)
	void CleanUp();
	
	
	UFUNCTION(CallInEditor, DisplayName = "Generate")
	void GenerateInEditor(); 
	
};
