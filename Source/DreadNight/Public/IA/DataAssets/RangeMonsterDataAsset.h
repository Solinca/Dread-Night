// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HostileMonsterDataAsset.h"
#include "RangeMonsterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API URangeMonsterDataAsset : public UHostileMonsterDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FleeRange;

public:
	float GetFleeRange() const;
};
