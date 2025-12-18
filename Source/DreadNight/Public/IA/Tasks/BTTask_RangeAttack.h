#pragma once

#include "CoreMinimal.h"
#include "BTTask_Base.h"
#include "BTTask_RangeAttack.generated.h"

struct FBTRangeAttackTaskMemory
{
	TSubclassOf<AActor> SpawnedProjectile;
	TWeakObjectPtr<AActor> AttackedTarget;
	TWeakObjectPtr<UAnimMontage> AttackAnimationMontage;
	TWeakObjectPtr<UAnimInstance> AnimInstance;

	FName ThrowingBoneName;
	
	FTimerHandle PlayMontageHandle;

	float AttackCooldown{ 0.f };
	float AttackDamage{ 0.f };

	bool bInitialized{ false };
};

UCLASS()
class DREADNIGHT_API UBTTask_RangeAttack : public UBTTask_Base
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Class SpawnedProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Float AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector AttackedTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Object AttackAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Name ThrowingBoneName;

public:
	UBTTask_RangeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	                            EBTNodeResult::Type TaskResult) override;

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual uint16 GetInstanceMemorySize() const override;

	UFUNCTION()
	void OnAttackNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	void LaunchMontageTimer(ACharacter* AICharacter, FBTRangeAttackTaskMemory* RangeAttackTaskMemory,
							UBehaviorTreeComponent* BehaviorTreeComponent, const float Rate);

	void OnPlayMontageTimerFinish(ACharacter* AICharacter, FBTRangeAttackTaskMemory* RangeAttackTaskMemory,
							UBehaviorTreeComponent* BehaviorTreeComponent);
	
	void BindEndAnimEvent(FBTRangeAttackTaskMemory* RangeAttackTaskMemory,
						  UBehaviorTreeComponent* BehaviorTreeComponent);
	
	void OnEndMontage(FBTRangeAttackTaskMemory* RangeAttackTaskMemory, UBehaviorTreeComponent* OwnerComp,
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
