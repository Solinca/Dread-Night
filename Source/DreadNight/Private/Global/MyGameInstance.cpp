// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/MyGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SaveSystem/DN_SaveGame.h"

void UMyGameInstance::Save(UWorld* World)
{
	if (SaveGame = Cast<UDN_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDN_SaveGame::StaticClass()));SaveGame)
	{
		SaveGame->GatherAllSaveData(World);
		UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName.ToString(), UserIndex);
	}
}

void UMyGameInstance::Load(UWorld* World)
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName.ToString(),UserIndex))
	{
		Save(World);
		return;
	}
	if (SaveGame = Cast<UDN_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName.ToString(), UserIndex));SaveGame)
	{ 
		SaveGame->UseAllSaveData(World); 
	}
}
