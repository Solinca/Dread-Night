// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterDataAsset.h"
#include "HostileMonsterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UHostileMonsterDataAsset : public UMonsterDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AcceptableRadius{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage;

public:
	[[nodiscard]] float GetAttackCooldown() const;
	[[nodiscard]] float GetAttackDamage() const;
	[[nodiscard]] float GetAttackRange() const;
	[[nodiscard]] float GetAcceptableRadius() const;
};
