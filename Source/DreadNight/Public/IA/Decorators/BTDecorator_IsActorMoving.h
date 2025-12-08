#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTDecorator_IsActorMoving.generated.h"

/**
 * Structure to store memory specific to the IsActorMoving decorator.
 */
struct FBTIsActorMovingDecoratorMemory
{
	bool bHasMoved{false};
	TWeakObjectPtr<AActor> Actor;
	FVector OldActorLocation;
	float TickInterval{0.f};

	bool bInitialized{false};
};

/**
 * Behavior Tree decorator that checks if an actor is moving.
 * This decorator monitors the movement of an actor and evaluates
 * whether the actor's position has changed over time.
 */
UCLASS()
class DREADNIGHT_API UBTDecorator_IsActorMoving : public UBTDecorator
{
	GENERATED_BODY()

protected:
	/** Blackboard key selector for the actor to monitor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector Actor;

	/** Blackboard key or value for the tick interval. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Float TickInterval;

public:
	UBTDecorator_IsActorMoving();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	virtual uint16 GetInstanceMemorySize() const override;

private:
	EBlackboardNotificationResult OnActorKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	EBlackboardNotificationResult OnTickIntervalKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

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
	FBTIsActorMovingDecoratorMemory* CastNodeMemory(uint8* NodeMemory) const;

	/**
	 * Checks if two vectors are nearly equal within a specified tolerance.
	 * @param First - The first vector.
	 * @param Second - The second vector.
	 * @param ErrorTolerance - The allowed error tolerance.
	 * @return True if the vectors are nearly equal, false otherwise.
	 */
	static bool IsNearlyEqual(const FVector& First, const FVector& Second, float ErrorTolerance = UE_SMALL_NUMBER);
};
