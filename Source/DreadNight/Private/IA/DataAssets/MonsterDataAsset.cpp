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

float UMonsterDataAsset::GetMaxHealth() const
{
	return MaxHealth;
}

const TObjectPtr<UDataTable>& UMonsterDataAsset::GetLootDataTable() const
{
	return LootDataTable;
}
