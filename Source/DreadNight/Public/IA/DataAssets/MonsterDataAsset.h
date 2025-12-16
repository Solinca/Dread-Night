#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/DataAsset.h"
#include "MonsterDataAsset.generated.h"

UCLASS(Blueprintable, BlueprintType, Category = "Data Asset", ClassGroup = "Data Asset")
class DREADNIGHT_API UMonsterDataAsset : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> LootDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxDroppedLootStack;
	
public:
	[[nodiscard]] USkeletalMesh* GetMesh() const;
	[[nodiscard]] UBehaviorTree* GetBehaviorTree() const;
	[[nodiscard]] const FName& GetName() const;
	[[nodiscard]] float GetMaxHealth() const;
	[[nodiscard]] const TObjectPtr<UDataTable>& GetLootDataTable() const;
	[[nodiscard]] int GetMaxDroppedLootStack() const;
	
};
