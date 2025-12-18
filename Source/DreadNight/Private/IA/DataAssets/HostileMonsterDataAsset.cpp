#include "IA/DataAssets/HostileMonsterDataAsset.h"

float UHostileMonsterDataAsset::GetAttackCooldown() const
{
	return AttackCooldown;
}

float UHostileMonsterDataAsset::GetAttackDamage() const
{
	return AttackDamage;
}

float UHostileMonsterDataAsset::GetAttackRange() const
{
	return AttackRange;
}

float UHostileMonsterDataAsset::GetAcceptableRadius() const
{
	return AcceptableRadius;
}

UAnimMontage* UHostileMonsterDataAsset::GetAttackAnimationMontage()
{
	return AttackAnimationMontage;
}
