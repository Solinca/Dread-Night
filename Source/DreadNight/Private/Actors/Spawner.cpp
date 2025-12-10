#include "Actors/Spawner.h"
#include "Subsystems/World/WaveWorldSubsystem.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	UWaveWorldSubsystem::Get(GetWorld())->RegisterSpawner(this);
}

ABaseAICharacter* ASpawner::SpawnCharacter(TSubclassOf<ABaseAICharacter> CharacterClass, UMonsterDataAsset* Data)
{
	FTransform AITransform;

	AITransform.SetLocation(GetActorLocation());

	AITransform.SetScale3D(FVector::One());

	AITransform.SetRotation(FQuat::Identity);

	ABaseAICharacter* Monster = GetWorld()->SpawnActorDeferred<ABaseAICharacter>(CharacterClass, AITransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	Monster->SetMonsterData(Data);

	Monster->FinishSpawning(AITransform);

	return Monster;
}
