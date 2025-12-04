// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTDecorator_IsActorMoving.generated.h"

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
 */
UCLASS()
class DREADNIGHT_API UBTDecorator_IsActorMoving : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector Actor;

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

	FBTIsActorMovingDecoratorMemory* CastNodeMemory(uint8* NodeMemory) const;

	static bool IsNearlyEqual(const FVector& First, const FVector& Second, float ErrorTolerance = UE_SMALL_NUMBER);
};
