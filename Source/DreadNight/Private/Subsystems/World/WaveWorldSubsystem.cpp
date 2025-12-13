#include "Subsystems/World/WaveWorldSubsystem.h"
#include "Subsystems/World/DayCycleSubSystem.h"
#include "Global/BaseLevelWorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Data/Wave/WaveSystemDataAsset.h"
#include "Data/DayCycleSystem/DayCycleSystemDataAsset.h"

void UWaveWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	BaseWorldSettings = Cast<ABaseLevelWorldSettings>(InWorld.GetWorldSettings());

	UDayCycleSubSystem::Get(&InWorld)->OnNightStart.AddDynamic(this, &UWaveWorldSubsystem::OnNightStart);

	NewWaveSound = BaseWorldSettings->WaveSystemData->NewWaveSound;
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
		for (FWaveMonsterData& Monster : BaseWorldSettings->WaveSystemData->WaveList[WaveIndex].MonsterList)
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
	}, NewWaveSound->GetDuration() * BaseWorldSettings->WaveSystemData->GlobalWaveSpawnDelay, false);
}

void UWaveWorldSubsystem::MonsterDeath(AActor* Monster)
{
	CurrentDeathCount++;

	if (CurrentDeathCount >= RequiredDeathCount)
	{
		OnWaveEnd.Broadcast();

		WaveIndex++;

		if (WaveIndex >= BaseWorldSettings->WaveSystemData->WaveList.Num())
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
