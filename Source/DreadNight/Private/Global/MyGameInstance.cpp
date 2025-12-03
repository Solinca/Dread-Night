// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/MyGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SaveSystem/DN_SaveGame.h"

void UMyGameInstance::Save()
{
	if (UDN_SaveGame* SaveGame = Cast<UDN_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDN_SaveGame::StaticClass())))
	{
		SaveGame->GatherAllSaveData();
		UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName.ToString(), UserIndex);
	}
}

void UMyGameInstance::Load()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName.ToString(),UserIndex))
	{
		Save();
		return;
	}
	if (UDN_SaveGame* SaveGame = Cast<UDN_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName.ToString(), UserIndex)))
	{
		SaveGame->UseAllSaveData(); 
	}
}
