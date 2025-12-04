// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class UDN_SaveGame;
/**
 * 
 */
UCLASS()
class DREADNIGHT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	FName SaveSlotName;
	
	UPROPERTY(EditDefaultsOnly)
	int UserIndex;

	UPROPERTY(Transient)
	UDN_SaveGame* SaveGame;
public:

	UFUNCTION(BlueprintCallable)
	void Save(UWorld* World);

	UFUNCTION(BlueprintCallable)
	void Load(UWorld* World);
};
