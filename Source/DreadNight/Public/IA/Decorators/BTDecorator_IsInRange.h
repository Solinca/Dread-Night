#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTDecorator_IsInRange.generated.h"

struct FBTIsInRangeDecoratorMemory
{
	TWeakObjectPtr<AActor> TargetActor;
	float Range{0.f};

	bool bInitialized{false};
};

/**
 * Behavior Tree decorator that checks if a target actor is within a specified range.
 */
UCLASS()
class DREADNIGHT_API UBTDecorator_IsInRange : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Float Range;
	
public:
	UBTDecorator_IsInRange();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual uint16 GetInstanceMemorySize() const override;
private:
	EBlackboardNotificationResult OnTargetActorKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	EBlackboardNotificationResult OnRangeKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

	FBTIsInRangeDecoratorMemory* CastNodeMemory(uint8* NodeMemory) const;
};
