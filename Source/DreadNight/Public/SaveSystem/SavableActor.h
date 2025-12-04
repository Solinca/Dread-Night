// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableActor.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USavableActor : public UInterface
{
	GENERATED_BODY()
};


/*Interface that implements all the necessary for saving an actor.
* Mark all the property that need to be saved with UPROPERTY(SaveGame).
* For actor reference, use the FSavableReference to ensure a safe access.
*
* For dynamic actor, be sure to call SetIsDynamicallySpawned with the UClass used for the spawn, to ensure that this
* actor is correctly replicated when loading a new save.
 */
class DREADNIGHT_API ISavableActor
{
	GENERATED_BODY()
 
public:
	virtual FName GetUniqueIdentifier() const = 0;

	virtual void OnPostLoad(const TMap<FName, ISavableActor*>& SavableActorCache);

	virtual void SetIsDynamicallySpawned(UClass* SpawnClass, FGuid UniqueID = FGuid::NewGuid()) = 0;
	virtual bool IsDynamicallySpawned() const = 0;
     
	virtual UClass* GetSpawnClass() const = 0;
	virtual FTransform GetSpawnTransform() const = 0;
};

#define GENERATE_GENERIC_SAVABLE_OBJECT()\
protected:\
bool bIsDynamicallySpawned = false;	\
UClass* DynamicSpawnedClass = nullptr;\
FGuid DynamicUniqueID;\
public:\
virtual FName GetUniqueIdentifier() const override {return bIsDynamicallySpawned ? FName(DynamicUniqueID.ToString()) : FName(GetName());};\
virtual void SetIsDynamicallySpawned(UClass* SpawnClass, FGuid UniqueID = FGuid::NewGuid()) override {\
DynamicUniqueID = MoveTemp(UniqueID);\
DynamicSpawnedClass = SpawnClass;\
bIsDynamicallySpawned = true;};\
virtual bool IsDynamicallySpawned() const override {return bIsDynamicallySpawned;};\
virtual UClass* GetSpawnClass() const override {return DynamicSpawnedClass;};\
virtual FTransform GetSpawnTransform() const override{return GetActorTransform();};\
