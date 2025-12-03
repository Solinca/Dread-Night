// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableObject.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USavableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DREADNIGHT_API ISavableObject
{
	GENERATED_BODY()
 
public:
	virtual FName GetUniqueIdentifier() const = 0;

	virtual void PopulateSaveArray(TArray<uint8>& OutData) = 0;
	virtual void UseSaveArray(TArray<uint8>& InData) = 0;

	virtual void SetIsDynamicallySpawner() = 0;
	virtual bool IsDynamicallySpawned() const = 0;
     
	virtual UClass* GetSpawnClass() const = 0;
	virtual FTransform GetSpawnTransform() const = 0;
};

#define GENERATE_GENERIC_SAVABLE_OBJECT()\
protected:\
bool bIsDynamicallySpawned = false;	\
public:\
virtual FName GetUniqueIdentifier() const override {return FName(GetName());};\
virtual void SetIsDynamicallySpawner() override {bIsDynamicallySpawned = true;};\
virtual bool IsDynamicallySpawned() const override {return bIsDynamicallySpawned;};\
virtual UClass* GetSpawnClass() const override {return StaticClass();};\
virtual FTransform GetSpawnTransform() const override{return GetActorTransform();};\
