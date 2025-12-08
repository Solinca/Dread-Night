// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/DataAsset.h"
#include "MonsterDataAsset.generated.h"

/**
 * 
 */
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
};
