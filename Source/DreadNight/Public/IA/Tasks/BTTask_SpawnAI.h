// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_Base.h"
#include "IA/DataAssets/FlyingMonsterDataAsset.h"
#include "BTTask_SpawnAI.generated.h"

struct FBTSpawnAITaskMemory
{
	TWeakObjectPtr<USpawnableAIContainer> SpawnableAI;
	TWeakObjectPtr<AActor> AttackedTarget;
	TWeakObjectPtr<UAnimMontage> AttackAnimationMontage;
	TWeakObjectPtr<UAnimInstance> AnimInstance;
	
	FTimerHandle PlayMontageHandle;

	float AttackCooldown{ 0.f };

	bool bInitialized{ false };
};

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UBTTask_SpawnAI : public UBTTask_Base
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Object SpawnableAI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Float AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector AttackedTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Object AttackAnimationMontage;

public:
	UBTTask_SpawnAI();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	                            EBTNodeResult::Type TaskResult) override;

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual uint16 GetInstanceMemorySize() const override;

	UFUNCTION()
	void OnAttackNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	void LaunchMontageTimer(ACharacter* AICharacter, FBTSpawnAITaskMemory* SpawnAITaskMemory,
							UBehaviorTreeComponent* BehaviorTreeComponent, const float Rate);

	void OnPlayMontageTimerFinish(ACharacter* AICharacter, FBTSpawnAITaskMemory* SpawnAITaskMemory,
							UBehaviorTreeComponent* BehaviorTreeComponent);
	
	void BindEndAnimEvent(FBTSpawnAITaskMemory* SpawnAITaskMemory,
						  UBehaviorTreeComponent* BehaviorTreeComponent);
	
	void OnEndMontage(FBTSpawnAITaskMemory* SpawnAITaskMemory, UBehaviorTreeComponent* OwnerComp,
	                  UAnimMontage* Montage, bool bInterrupted);

private:
	EBlackboardNotificationResult OnSpawnedProjectileKeyValueChange(const UBlackboardComponent& Blackboard,
	                                                                FBlackboard::FKey ChangedKeyID);
	EBlackboardNotificationResult OnAttackCooldownKeyValueChange(const UBlackboardComponent& Blackboard,
	                                                             FBlackboard::FKey ChangedKeyID);
	EBlackboardNotificationResult OnAttackDamageKeyValueChange(const UBlackboardComponent& Blackboard,
	                                                           FBlackboard::FKey ChangedKeyID);
	EBlackboardNotificationResult OnAttackedTargetKeyValueChange(const UBlackboardComponent& Blackboard,
	                                                             FBlackboard::FKey ChangedKeyID);
};
