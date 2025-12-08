// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/DataAssets/BlackboardMonsterDataAsset.h"

float UBlackboardMonsterDataAsset::GetAttackRange() const
{
	return AttackRange;
}

float UBlackboardMonsterDataAsset::GetAcceptableRadius() const
{
	return AcceptableRadius;
}
