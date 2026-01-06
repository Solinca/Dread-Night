// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterDataAsset.h"
#include "IA/Characters/BaseAICharacter.h"
#include "FlyingMonsterDataAsset.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FSpawnableData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseAICharacter> AIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMonsterDataAsset> AIData;
};

UCLASS(Blueprintable, BlueprintType)
class DREADNIGHT_API USpawnableAIContainer : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnableData> SpawnableAI;
};

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UFlyingMonsterDataAsset : public UMonsterDataAsset
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
	float FleeRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> AttackAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpawnableAIContainer> SpawnableAI;

public:
	[[nodiscard]] float GetAttackRange() const;
	[[nodiscard]] float GetAcceptableRadius() const;
	[[nodiscard]] float GetAttackCooldown() const;
	[[nodiscard]] float GetFleeRange() const;
	[[nodiscard]] UAnimMontage* GetAttackAnimationMontage() const;
	[[nodiscard]] USpawnableAIContainer* GetSpawnableAI() const;
};
