// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "DN_SaveGame.generated.h"

class ISavableActor;


USTRUCT() 
struct FSaveDataStruct
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName Identifier;

	UPROPERTY()
	TArray<uint8> Data;
 
	UPROPERTY()
	bool bIsDynamicActor = false;
    
	UPROPERTY()
	TSubclassOf<AActor> ActorClass;
    
	UPROPERTY()
	FTransform SpawnTransform;
};

USTRUCT() 
struct FUniqueObjectSave
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName Identifier;

	UPROPERTY()
	TArray<uint8> Data; 
};

USTRUCT()
struct FGameSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TArray<FSaveDataStruct> GameData;
};

UCLASS()
class DREADNIGHT_API UDN_SaveGame : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY()
	FGameSaveData GameSaveData;

	UPROPERTY()
	TArray<FUniqueObjectSave> WorldSubsystemSave;
	
	void CollectSaveData(UWorld* WorldContext);

	TMap<FName, AActor*> BuildWorldActorCache(UWorld* WorldContext) const;
	TMap<FName, ISavableActor*> BuildWorldSavableCache(UWorld* WorldContext) const;

	void SerializeActorComponents(AActor* Actor, FObjectAndNameAsStringProxyArchive& Ar);
	void SerializeWorldSubsystem(UWorld* World);
	void DeserializeWorldSubsystem(UWorld* World);

	void DeserializeActor(UWorld* World);
public:
	UFUNCTION(BlueprintCallable)
	void GatherAllSaveData(UWorld* WorldContext);
	
	UFUNCTION(BlueprintCallable)
	void UseAllSaveData(UWorld* WorldContext);


	UPROPERTY(BlueprintReadWrite)
	int Seed;
};
