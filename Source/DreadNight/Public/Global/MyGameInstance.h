// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class UDN_SaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPCGEndGenerationSignature);

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

	int Seed;
	bool bIsNewGame = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
	bool bIsFirstLaunch = true;
public:

	UFUNCTION(BlueprintCallable)
	void NewGame();

	UFUNCTION(BlueprintCallable)
	void Save(UWorld* World);

	UFUNCTION(BlueprintCallable)
	void Load(UWorld* World);

	UFUNCTION(BlueprintCallable)
	bool IsNewGame();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPCGEndGenerationSignature OnPCGEndGeneration;

	UFUNCTION(BlueprintCallable)
	int GetSeed();
};
