// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/DataAssets/FlyingMonsterDataAsset.h"

float UFlyingMonsterDataAsset::GetAttackRange() const
{
	return AttackRange;
}

float UFlyingMonsterDataAsset::GetAcceptableRadius() const
{
	return AcceptableRadius;
}

float UFlyingMonsterDataAsset::GetAttackCooldown() const
{
	return AttackCooldown;
}

float UFlyingMonsterDataAsset::GetFleeRange() const
{
	return FleeRange;
}

UAnimMontage* UFlyingMonsterDataAsset::GetAttackAnimationMontage() const
{
	return AttackAnimationMontage;
}

USpawnableAIContainer* UFlyingMonsterDataAsset::GetSpawnableAI() const
{
	return SpawnableAI;
}
