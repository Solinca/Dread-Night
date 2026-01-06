// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "FlyingAICharacter.generated.h"

UCLASS()
class DREADNIGHT_API AFlyingAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()
	
protected:
	virtual void OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset) override;
};
