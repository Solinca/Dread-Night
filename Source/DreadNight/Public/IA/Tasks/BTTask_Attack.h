#pragma once

#include "CoreMinimal.h"
#include "BTTask_Base.h"
#include "BTTask_Attack.generated.h"

struct FBTAttackTaskMemory
{
	TWeakObjectPtr<AActor> AttackedTarget;
	TWeakObjectPtr<UAnimMontage> AttackAnimationMontage;
	TWeakObjectPtr<UAnimInstance> AnimInstance;

	FTimerHandle PlayMontageHandle;
	
	float AttackCooldown{0.f};
	float AttackDamage{0.f};
	
	bool bInitialized{false};
};

UCLASS()
class DREADNIGHT_API UBTTask_Attack : public UBTTask_Base
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Float AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector AttackedTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Object AttackAnimationMontage;
	
public:
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	void LaunchMontageTimer(ACharacter* AICharacter, FBTAttackTaskMemory* AttackTaskMemory,
							UBehaviorTreeComponent* BehaviorTreeComponent, const float Rate);
	
	UFUNCTION()
	void OnAttackNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	void OnPlayMontageTimerFinish(ACharacter* AICharacter, FBTAttackTaskMemory* AttackTaskMemory,
						UBehaviorTreeComponent* BehaviorTreeComponent);

	void BindEndAnimEvent(FBTAttackTaskMemory* AttackTaskMemory,
					  UBehaviorTreeComponent* BehaviorTreeComponent);

	void OnEndMontage(FBTAttackTaskMemory* AttackTaskMemory, UBehaviorTreeComponent* OwnerComp,
				  UAnimMontage* Montage, bool bInterrupted);

	virtual uint16 GetInstanceMemorySize() const override;
private:
	EBlackboardNotificationResult OnAttackedTargetKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	EBlackboardNotificationResult OnAttackCooldownKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	EBlackboardNotificationResult OnAttackDamageKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
};
