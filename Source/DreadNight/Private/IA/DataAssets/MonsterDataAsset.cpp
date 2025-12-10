// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/DataAssets/MonsterDataAsset.h"

USkeletalMesh* UMonsterDataAsset::GetMesh() const
{
	return Mesh;
}

UBehaviorTree* UMonsterDataAsset::GetBehaviorTree() const
{
	return BehaviorTree;
}

const FName& UMonsterDataAsset::GetName() const
{
	return Name;
}
