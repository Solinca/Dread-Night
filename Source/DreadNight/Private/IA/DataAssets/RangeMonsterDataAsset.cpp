#include "IA/DataAssets/RangeMonsterDataAsset.h"

TSubclassOf<AProjectileActor> URangeMonsterDataAsset::GetSpawnedProjectile() const
{
	return SpawnedProjectile;
}

float URangeMonsterDataAsset::GetFleeRange() const
{
	return FleeRange;
}

const FName& URangeMonsterDataAsset::GetThrowBoneName() const
{
	return ThrowBoneName;
}
