#include "Global/MyGameInstance.h"
#include "Global/MyGameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/DN_SaveGame.h"

void UMyGameInstance::Init()
{
	Super::Init();

	// Wait 0.1f seconds so the Sound Component in Unreal can Init

	FTimerHandle WaitForSoundInit;

	GetWorld()->GetTimerManager().SetTimer(WaitForSoundInit, [this]
	{
		UMyGameUserSettings* MySettings = Cast<UMyGameUserSettings>(GEngine->GetGameUserSettings());

		MySettings->LoadSettings();
		
		if (MySettings->GetLastCPUBenchmarkResult() < 0)
		{
			MySettings->RunHardwareBenchmark();
			MySettings->ApplyHardwareBenchmarkResults();
		}
		else
		{
			MySettings->ApplySettings(false);
		}

		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MusicSoundMix, MusicSoundClass, MySettings->GetMusicVolume() / 100, 1, 0);

		UGameplayStatics::PushSoundMixModifier(GetWorld(), MusicSoundMix);

		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SFXSoundMix, SFXSoundClass, MySettings->GetSFXVolume() / 100, 1, 0);

		UGameplayStatics::PushSoundMixModifier(GetWorld(), SFXSoundMix);
	}, 0.1f, false);
}

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
