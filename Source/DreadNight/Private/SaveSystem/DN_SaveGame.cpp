// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/DN_SaveGame.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SavableActor.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UObject/UnrealTypePrivate.h"


void UDN_SaveGame::CollectSaveData(UWorld* WorldContext)
{
    for (TActorIterator<AActor> It(WorldContext); It; ++It)
	{
		if (!It->Implements<USavableActor>())
		{
			continue;
		}

    	FSaveDataStruct ActorSaveInfo;

    	ISavableActor* SavableObject = Cast<ISavableActor>(*It);
    	SavableObject->OnPreSave();
    	
    	ActorSaveInfo.bIsDynamicActor = SavableObject->IsDynamicallySpawned();
    	ActorSaveInfo.SpawnTransform = SavableObject->GetSpawnTransform();
    	ActorSaveInfo.Identifier = SavableObject->GetUniqueIdentifier();		
    	if (ActorSaveInfo.bIsDynamicActor)
    	{
    		ActorSaveInfo.ActorClass = SavableObject->GetSpawnClass();
    	}
		
    	
    	FMemoryWriter MemoryWriter(ActorSaveInfo.Data, true);
    	FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, false);
    	Ar.ArIsSaveGame = true; 
    	It->Serialize(Ar);
    	SerializeActorComponents(*It, Ar);

    	GameSaveData.GameData.Add(ActorSaveInfo);
	}
}

TMap<FName, AActor*> UDN_SaveGame::BuildWorldActorCache(UWorld* WorldContext) const
{
	TMap<FName, AActor*> ActorCache;
	for (TActorIterator<AActor> It(WorldContext); It; ++It)
	{
		if (!It->Implements<USavableActor>())
		{
			continue;
		}

		ISavableActor* Object = Cast<ISavableActor>(*It);
		if (!Object->IsDynamicallySpawned())
		{
			ActorCache.Add(Object->GetUniqueIdentifier(), *It);
		}
	}

	return MoveTemp(ActorCache);	
}

TMap<FName, ISavableActor*> UDN_SaveGame::BuildWorldSavableCache(UWorld* WorldContext) const
{
	TMap<FName, ISavableActor*> SavableCache;
	for (TActorIterator<AActor> It(WorldContext); It; ++It)
	{
		if (!It->Implements<USavableActor>())
		{
			continue;
		}

		ISavableActor* Object = Cast<ISavableActor>(*It);
		SavableCache.Add(Object->GetUniqueIdentifier(), Object); 
	}

	return MoveTemp(SavableCache);	
}

void UDN_SaveGame::SerializeActorComponents(AActor* Actor, FObjectAndNameAsStringProxyArchive& Ar)
{
	for (TFieldIterator<FProperty> PropIt(Actor->GetClass()); PropIt; ++PropIt)
	{
		if (!PropIt->HasAnyPropertyFlags(CPF_SaveGame))
		{
			continue;
		}
		
		if (FObjectProperty* ObjProp = CastField<FObjectProperty>(*PropIt))
		{
			UObject* ObjValue = ObjProp->GetObjectPropertyValue_InContainer(Actor);

			if (UActorComponent* Comp = Cast<UActorComponent>(ObjValue))
			{
				Comp->Serialize(Ar);
			}
		}
	}
}

void UDN_SaveGame::GatherAllSaveData(UWorld* WorldContext)
{
	GameSaveData.GameData.Empty();
	CollectSaveData(WorldContext);	
}

void UDN_SaveGame::UseAllSaveData(UWorld* WorldContext)
{
	TMap<FName, AActor*> ActorCache = BuildWorldActorCache(WorldContext);
	for (FSaveDataStruct& SaveActorData : GameSaveData.GameData)
	{
		AActor* TargetActor = nullptr;
		if (!SaveActorData.bIsDynamicActor && ActorCache.Contains(SaveActorData.Identifier))
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
            
			TargetActor = WorldContext->SpawnActor<AActor>(
				SaveActorData.ActorClass,
				SaveActorData.SpawnTransform,
				SpawnParams
			);
			if (ISavableActor* SavableActor = Cast<ISavableActor>(TargetActor))
			{
				FGuid ActorGuid;
				FGuid::Parse(SaveActorData.Identifier.ToString(),ActorGuid);
				SavableActor->SetIsDynamicallySpawned(SaveActorData.ActorClass, ActorGuid);
			}
		}

		if (TargetActor && TargetActor->Implements<USavableActor>())
		{ 
			FMemoryReader MemoryReader(SaveActorData.Data, true);
			FObjectAndNameAsStringProxyArchive Ar(MemoryReader, false);
    		Ar.ArIsSaveGame = true;
			TargetActor->Serialize(Ar);
			SerializeActorComponents(TargetActor, Ar);
		}
	}
	TMap<FName, ISavableActor*> FinalSavableCache = BuildWorldSavableCache(WorldContext);
	for (TPair<FName, ISavableActor*>& Savable : FinalSavableCache)
	{
		Savable.Value->OnPostLoad(FinalSavableCache);
	}
}
