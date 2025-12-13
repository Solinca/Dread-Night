// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/DataAssets/RangeMonsterDataAsset.h"

TSubclassOf<AProjectileActor> URangeMonsterDataAsset::GetSpawnedProjectile() const
{
	return SpawnedProjectile;
}

float URangeMonsterDataAsset::GetFleeRange() const
{
	return FleeRange;
}
