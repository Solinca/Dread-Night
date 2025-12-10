#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Actors/Spawner.h"
#include "IA/Characters/BaseAICharacter.h"
#include "WaveWorldSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveEndSignature);

class ABaseLevelWorldSettings;

UCLASS()
class DREADNIGHT_API UWaveWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	ABaseLevelWorldSettings* BaseWorldSettings = nullptr;

	UFUNCTION()
	void OnNightStart();

	TArray<TWeakObjectPtr<ASpawner>> SpawnerList;

	int WaveIndex = 0;

	int RequiredDeathCount = 0;

	int CurrentDeathCount = 0;

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:
	FOnWaveEndSignature OnWaveEnd;

	void RegisterSpawner(ASpawner* Spawn);

	UFUNCTION()
	void MonsterDeath(AActor* Monster);

	static ThisClass* Get(UObject* WorldContext);
};
