// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlackboardMonsterDataAsset.h"
#include "RangeMonsterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API URangeMonsterDataAsset : public UBlackboardMonsterDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FleeRange;

public:
	float GetFleeRange() const;
};
