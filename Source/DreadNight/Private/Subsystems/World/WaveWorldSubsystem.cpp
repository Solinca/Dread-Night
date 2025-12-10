#include "Subsystems/World/WaveWorldSubsystem.h"
#include "Subsystems/World/DayCycleSubSystem.h"
#include "Global/BaseLevelWorldSettings.h"
#include "Data/Wave/WaveDataAsset.h"

void UWaveWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	BaseWorldSettings = Cast<ABaseLevelWorldSettings>(InWorld.GetWorldSettings());

	UDayCycleSubSystem::Get(&InWorld)->OnNightStart.AddDynamic(this, &UWaveWorldSubsystem::OnNightStart);
}

bool UWaveWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return World && World->IsGameWorld() && World == Cast<ABaseLevelWorldSettings>(World->GetWorldSettings())->MapToLoadSubsystems;
}

void UWaveWorldSubsystem::OnNightStart()
{
	CurrentDeathCount = 0;

	for (FWaveMonsterData& Monster : BaseWorldSettings->WaveList[WaveIndex]->MonsterList)
	{
		RequiredDeathCount += Monster.Count;

		for (int i = 0; i < Monster.Count; i++)
		{
			int SpawnerIndex = FMath::RandRange(0, SpawnerList.Num() - 1);

			ABaseAICharacter* SpawnedMonster = SpawnerList[SpawnerIndex]->SpawnCharacter(Monster.CharacterClass, Monster.Data);

			SpawnedMonster->OnDestroyed.AddDynamic(this, &UWaveWorldSubsystem::MonsterDeath);
		}
	}
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
