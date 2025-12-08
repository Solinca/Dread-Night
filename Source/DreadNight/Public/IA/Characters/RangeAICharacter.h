// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "RangeAICharacter.generated.h"

UCLASS(BlueprintType, Blueprintable, Category = "AI", ClassGroup = "AICharacter")
class DREADNIGHT_API ARangeAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()

protected:
	virtual void OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset) override;
};
