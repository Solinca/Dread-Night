// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveSystem/SavableObject.h"
#include "TestActor.generated.h"

UCLASS()
class DREADNIGHT_API ATestActor : public AActor, public ISavableObject
{
public:
	virtual void PopulateSaveArray(TArray<uint8>& OutData) override;
	virtual void UseSaveArray(TArray<uint8>& InData) override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	GENERATE_GENERIC_SAVABLE_OBJECT()
};
