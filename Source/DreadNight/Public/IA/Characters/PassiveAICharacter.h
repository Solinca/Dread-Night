// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "SaveSystem/SavableActor.h"
#include "PassiveAICharacter.generated.h"

class URespawnComponent;


UCLASS()
class DREADNIGHT_API APassiveAICharacter : public ABaseAICharacter, public ISavableActor
{
	GENERATED_BODY()
	GENERATE_GENERIC_SAVABLE_OBJECT()

private:
	FTimerHandle FleeTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URespawnComponent> RespawnComponent;
	
protected:
	APassiveAICharacter();

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset) override;

	virtual bool TryApplyDamage(float Damage, AActor* DamageInstigator) override;

	virtual void OnDeath() override;

private:
	void OnFleeTimerFinish();
};
