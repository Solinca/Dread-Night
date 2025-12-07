// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "SavableActor.h"
#include "SavableReference.generated.h"

/*Use this struct when you have a reference to an actor that need to be saved*/
USTRUCT()
struct DREADNIGHT_API FSavableReference
{
	GENERATED_BODY()
	
	UPROPERTY(SaveGame)
	FName UniqueID = FName();

	ISavableActor* Actor = nullptr;

	bool TryFindNewSavableInCache(const TMap<FName, ISavableActor*>& Cache)
	{
		return UniqueID.IsNone()
			&& Cache.Contains(UniqueID)
			&& AssignNewSavable(Cache[UniqueID]);
	}
	
	bool AssignNewActor(AActor* Other)
	{
		if (!Other)
		{
			Actor = nullptr;
			UniqueID = FName();
			return false;
		}
		
		if (!Other->Implements<USavableActor>())
		{
			UE_LOG(LogTemp, Error, TEXT("%s doesn't implements SavableActor interface !"), *Other->GetName());
			return false;
		}
		Actor = Cast<ISavableActor>(Other);
		UniqueID = Actor->GetUniqueIdentifier();
		
		return true;
	}

	FORCEINLINE bool AssignNewSavable(ISavableActor* Other)
	{
		if (!Other)
		{
			Actor = nullptr;
			UniqueID = FName();
			return false;
		}
		
		Actor = Cast<ISavableActor>(Other);
		UniqueID = Actor->GetUniqueIdentifier();
		return true;
	}	
	
	template<typename T>
	FORCEINLINE T* FastGet()
	{
		return static_cast<T*>(Actor);
	}
	
	template<typename T>
	FORCEINLINE T* Get()
	{
		return Cast<T>(Actor);
	}

	bool TryReloadActor(const UWorld* WorldContext)
	{
		if (UniqueID.IsNone())
			return false;
		  
		for (TActorIterator<AActor> It(WorldContext); It; ++It)
		{
			if (!It->Implements<USavableActor>())
			{
				continue;
			}

			if (ISavableActor* Object = Cast<ISavableActor>(*It); Object->GetUniqueIdentifier() == UniqueID)
			{
				Actor = Object;
				return true;
			}
		}
		return false;
	}

	template<typename T>
	T* operator->()
	{
		return FastGet<T>();
	}

	FSavableReference& operator=(AActor* Other)
	{
		AssignNewActor(Other);
		return *this;
	}

	FSavableReference& operator=(const TMap<FName, ISavableActor*>& Cache)
	{
		TryFindNewSavableInCache(Cache);
		return *this;
	}

	FORCEINLINE FSavableReference& operator=(ISavableActor* Other)
	{
		AssignNewSavable(Other);
		return *this;
	}

	FORCEINLINE bool operator==(std::nullptr_t) const
	{
		return Actor == nullptr;
	}

	FORCEINLINE bool operator!=(std::nullptr_t) const
	{
		return Actor != nullptr;
	}

	FORCEINLINE operator bool() const
	{
		return Actor != nullptr && Actor->_getUObject()->IsValidLowLevel();
	}
};
