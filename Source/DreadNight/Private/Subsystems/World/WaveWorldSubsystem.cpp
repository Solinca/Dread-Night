#include "Subsystems/World/WaveWorldSubsystem.h"

#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Subsystems/World/DayCycleSubSystem.h"
#include "Global/BaseLevelWorldSettings.h"
#include "Data/Wave/WaveDataAsset.h"
#include "Kismet/GameplayStatics.h"

void UWaveWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	BaseWorldSettings = Cast<ABaseLevelWorldSettings>(InWorld.GetWorldSettings());

	UDayCycleSubSystem::Get(&InWorld)->OnNightStart.AddDynamic(this, &UWaveWorldSubsystem::OnNightStart);

	NewWaveSound = BaseWorldSettings->SoundsToPlay["NewWave"];
}

bool UWaveWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return World && World->IsGameWorld() && World == Cast<ABaseLevelWorldSettings>(World->GetWorldSettings())->MapToLoadSubsystems;
}

void UWaveWorldSubsystem::OnNightStart()
{
	UGameplayStatics::PlaySound2D(this, NewWaveSound);
	CurrentDeathCount = 0;
	RequiredDeathCount = 0;

	GetWorld()->GetTimerManager().SetTimer(WaveSpawnDelayTimerHandle, [this, World = GetWorld()]
	{
		for (FWaveMonsterData& Monster : BaseWorldSettings->WaveList[WaveIndex]->MonsterList)
		{
			RequiredDeathCount += Monster.Count;

			for (int i = 0; i < Monster.Count; i++)
			{
				const int SpawnerIndex = FMath::RandRange(0, SpawnerList.Num() - 1);

				if (ABaseAICharacter* SpawnedMonster = SpawnerList[SpawnerIndex]->SpawnCharacter(Monster.CharacterClass, Monster.Data))
				{
					SpawnedMonster->OnDestroyed.AddDynamic(World->GetSubsystem<UWaveWorldSubsystem>(), &UWaveWorldSubsystem::MonsterDeath);
				}
			}
		}
	}, NewWaveSound->GetDuration() * WaveSpawnDelay, false);
}

void UWaveWorldSubsystem::MonsterDeath(AActor* Monster)
{
	CurrentDeathCount++;

	if (CurrentDeathCount >= RequiredDeathCount)
	{
		OnWaveEnd.Broadcast();

		WaveIndex++;

		if (WaveIndex >= BaseWorldSettings->WaveList.Num())
		{
			WaveIndex = 0;

			//TODO: Increase difficulty
		}
	}
}

void UWaveWorldSubsystem::RegisterSpawner(ASpawner* Spawn)
{
	SpawnerList.Push(Spawn);
}

UWaveWorldSubsystem::ThisClass* UWaveWorldSubsystem::Get(UObject* WorldContext)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World->GetSubsystem<UWaveWorldSubsystem>();
	}

	return nullptr;
}
