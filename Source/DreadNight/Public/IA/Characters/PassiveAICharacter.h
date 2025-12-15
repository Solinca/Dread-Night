// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "PassiveAICharacter.generated.h"

UCLASS()
class DREADNIGHT_API APassiveAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()

private:
	FTimerHandle FleeTimerHandle; 
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset) override;

	virtual bool TryApplyDamage(float Damage, AActor* DamageInstigator) override;

private:
	void OnFleeTimerFinish();
};
