// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/DN_SaveGame.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SavableObject.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


void UDN_SaveGame::CollectSaveData()
{
    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (!It->Implements<USavableObject>())
		{
			continue;
		}

    	FSaveDataStruct ActorSaveInfo;

    	ISavableObject* SavableObject = Cast<ISavableObject>(*It);
    	
    	ActorSaveInfo.bIsDynamicActor = SavableObject->IsDynamicallySpawned();
    	ActorSaveInfo.SpawnTransform = SavableObject->GetSpawnTransform();
    	if (ActorSaveInfo.bIsDynamicActor)
    	{
    		ActorSaveInfo.ActorClass = SavableObject->GetSpawnClass();
    	}
	    else
	    {
    		ActorSaveInfo.Identifier = SavableObject->GetUniqueIdentifier();		    
	    }
		
    	
    	FMemoryWriter MemoryWriter(ActorSaveInfo.Data, true);
    	FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, false);
    	It->Serialize(Ar);

    	GameSaveData.GameData.Add(ActorSaveInfo);
	}
}

TMap<FName, AActor*> UDN_SaveGame::BuildWorldActorCache() const
{
	TMap<FName, AActor*> ActorCache;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (!It->Implements<USavableObject>())
		{
			continue;
		}

		ISavableObject* Object = Cast<ISavableObject>(*It);
		if (!Object->IsDynamicallySpawned())
		{
			ActorCache.Add(Object->GetUniqueIdentifier(), *It);
		}
	}

	return MoveTemp(ActorCache);	
}

void UDN_SaveGame::GatherAllSaveData()
{
	CollectSaveData();
	
}

void UDN_SaveGame::UseAllSaveData()
{
	TMap<FName, AActor*> ActorCache = BuildWorldActorCache();
	for (FSaveDataStruct& SaveActorData : GameSaveData.GameData)
	{
		AActor* TargetActor = nullptr;
		if (!SaveActorData.bIsDynamicActor)
		{
			TargetActor = ActorCache[SaveActorData.Identifier];
			TargetActor->SetActorTransform(SaveActorData.SpawnTransform);
		}
		else
		{
			if (!SaveActorData.ActorClass) continue;
            
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = 
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            
			TargetActor = GetWorld()->SpawnActor<AActor>(
				SaveActorData.ActorClass,
				SaveActorData.SpawnTransform,
				SpawnParams
			);
		}

		if (TargetActor && TargetActor->Implements<USavableObject>())
		{ 
			FMemoryReader MemoryReader(SaveActorData.Data, true);
			FObjectAndNameAsStringProxyArchive Ar(MemoryReader, false);
			TargetActor->Serialize(Ar); 
		}
	}
}
