// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/MyGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SaveSystem/DN_SaveGame.h"
 

void UMyGameInstance::NewGame()
{
	Seed = FMath::RandRange(0,999999);
	bIsNewGame = true;
}

void UMyGameInstance::Save(UWorld* World)
{
	if (SaveGame = Cast<UDN_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDN_SaveGame::StaticClass()));SaveGame)
	{
		SaveGame->GatherAllSaveData(World);
		SaveGame->Seed = Seed;
		UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName.ToString(), UserIndex);
	}
	bIsNewGame = false;
}

void UMyGameInstance::Load(UWorld* World)
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName.ToString(),UserIndex))
	{
		NewGame();
		return;
	}
	if (SaveGame = Cast<UDN_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName.ToString(), UserIndex));SaveGame)
	{ 
		SaveGame->UseAllSaveData(World);
		Seed = SaveGame->Seed;
		
	}
}

bool UMyGameInstance::IsNewGame()
{
	return bIsNewGame;	
}

int UMyGameInstance::GetSeed()
{
	return Seed;
}

 
