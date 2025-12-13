#pragma once

#include "CoreMinimal.h"
#include "BTDecorator_Base.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTDecorator_IsActorMoving.generated.h"

/**
 * Structure to store memory specific to the IsActorMoving decorator.
 */
struct FBTIsActorMovingDecoratorMemory
{
	TWeakObjectPtr<AActor> Actor;
	FVector OldActorLocation;
	float TickInterval{0.f};
	bool bHasMoved{false};

	bool bInitialized{false};
};

/**
 * Behavior Tree decorator that checks if an actor is moving.
 * This decorator monitors the movement of an actor and evaluates
 * whether the actor's position has changed over time.
 */
UCLASS()
class DREADNIGHT_API UBTDecorator_IsActorMoving : public UBTDecorator_Base
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
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual uint16 GetInstanceMemorySize() const override;

private:
	EBlackboardNotificationResult OnActorKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	EBlackboardNotificationResult OnTickIntervalKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	
	/**
	 * Checks if two vectors are nearly equal within a specified tolerance.
	 * @param First - The first vector.
	 * @param Second - The second vector.
	 * @param ErrorTolerance - The allowed error tolerance.
	 * @return True if the vectors are nearly equal, false otherwise.
	 */
	static bool IsNearlyEqual(const FVector& First, const FVector& Second, float ErrorTolerance = UE_SMALL_NUMBER);
};
