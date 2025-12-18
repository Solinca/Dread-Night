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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnPreLoad();
	virtual void OnPostLoad();
	virtual void OnPreSave();
	virtual void OnPostSave();
};

template<typename T>
requires std::is_base_of_v<UObject, T>
struct TSavableObject
{
	T* Object = nullptr;
	ISavableObject* SavableObject = nullptr;

	TSavableObject(T* NewObject)
	{
		if (!NewObject->template Implements<USavableObject>())
		{
			return;
		}
		Object = NewObject;
		SavableObject = Cast<ISavableObject>(NewObject);		
	}

	bool IsValid() const
	{
		return Object != nullptr;
	}
};