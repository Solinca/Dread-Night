#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTDecorator_IsInRange.generated.h"

/**
 * Structure to store memory specific to the IsInRange decorator.
 */
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

	/**
	 * The way Unreal works in Behavior Trees is by using 'Node Memory'.
	 * Node Memory is a simple data structure allocated on the heap by the engine for every AI agent using the decorator.
	 * GetInstanceMemorySize returns the size of FBTIsActorMovingDecoratorMemory to allocate.
	 * This way, the engine can use one single decorator instance for multiple AI agents by only changing the node memory address.
	 * 
	 * It optimises memory usage by avoiding the allocation of multiple decorators per AI, which would consume more memory.
	 * Note that you are responsible for managing the values within the Node Memory the engine only handles its allocation and deallocation.
	 *
	 * Unreal store the node memory in uint8 pointer for genericity, and then you cast it to your data struct.
	 *
	 * And Blueprint doesn't use the node memory pattern and instead instantiate one decorator for every IA agent.
	 */
	FBTIsInRangeDecoratorMemory* CastNodeMemory(uint8* NodeMemory) const;
};
