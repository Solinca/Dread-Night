// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Decorators/BTDecorator_IsActorMoving.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_IsActorMoving::UBTDecorator_IsActorMoving()
{
	NodeName = "Is Actor Moving";

	//bTickIntervals = true;
	bNotifyTick = true;
}

void UBTDecorator_IsActorMoving::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTIsActorMovingDecoratorMemory* ActorMovingDecoratorMemory{CastNodeMemory(NodeMemory)};
	if (ActorMovingDecoratorMemory->bInitialized)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};

	ActorMovingDecoratorMemory->Actor = Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(Actor.GetSelectedKeyID()));
	ActorMovingDecoratorMemory->OldActorLocation = ActorMovingDecoratorMemory->Actor->GetActorLocation();
	ActorMovingDecoratorMemory->TickInterval = TickInterval.GetValue(OwnerComp);
	ActorMovingDecoratorMemory->bInitialized = true;

	BlackboardComponent->RegisterObserver(Actor.GetSelectedKeyID(), this,
		FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_IsActorMoving::OnActorKeyValueChange));

	FBlackboard::FKey TickIntervalKey{TickInterval.GetKeyId(OwnerComp)};
	if (TickIntervalKey != FBlackboard::InvalidKey)
	{
		BlackboardComponent->RegisterObserver(TickIntervalKey, this,
			FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_IsActorMoving::OnTickIntervalKeyValueChange));
	}

	SetNextTickTime(NodeMemory, ActorMovingDecoratorMemory->TickInterval);
}

void UBTDecorator_IsActorMoving::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTIsActorMovingDecoratorMemory* ActorMovingDecoratorMemory{CastNodeMemory(NodeMemory)};
	ActorMovingDecoratorMemory->bHasMoved = false;
	ActorMovingDecoratorMemory->bInitialized = false;
	
	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	BlackboardComponent->UnregisterObserversFrom(this);
}

bool UBTDecorator_IsActorMoving::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const FBTIsActorMovingDecoratorMemory* ActorMovingDecoratorMemory{CastNodeMemory(NodeMemory)};

	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	const AActor* RetrievedActor{Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(Actor.GetSelectedKeyID()))};
	if (!RetrievedActor)
	{
		return false;
	}
	
	return ActorMovingDecoratorMemory->bHasMoved;
}

void UBTDecorator_IsActorMoving::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTIsActorMovingDecoratorMemory* ActorMovingDecoratorMemory{CastNodeMemory(NodeMemory)};
	if (!ActorMovingDecoratorMemory->bInitialized)
	{
		return;
	}

	FVector NewOldActorLocation{ActorMovingDecoratorMemory->Actor->GetActorLocation()};
	if (!IsNearlyEqual(NewOldActorLocation, ActorMovingDecoratorMemory->OldActorLocation))
	{
		ActorMovingDecoratorMemory->bHasMoved = true;
		OwnerComp.RequestExecution(this);
	}

	ActorMovingDecoratorMemory->OldActorLocation = NewOldActorLocation;

	SetNextTickTime(NodeMemory, ActorMovingDecoratorMemory->TickInterval);
}

uint16 UBTDecorator_IsActorMoving::GetInstanceMemorySize() const
{
	return sizeof(FBTIsActorMovingDecoratorMemory);
}

EBlackboardNotificationResult UBTDecorator_IsActorMoving::OnActorKeyValueChange(const UBlackboardComponent& Blackboard,
	FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorTreeComponent{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	uint8* NodeMemory{BehaviorTreeComponent->GetNodeMemory(this, BehaviorTreeComponent->FindInstanceContainingNode(this))};
	FBTIsActorMovingDecoratorMemory* IsInRangeDecoratorMemory{CastNodeMemory(NodeMemory)};
	IsInRangeDecoratorMemory->Actor = Cast<AActor>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));

	BehaviorTreeComponent->RequestExecution(this);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTDecorator_IsActorMoving::OnTickIntervalKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorTreeComponent{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	uint8* NodeMemory{BehaviorTreeComponent->GetNodeMemory(this, BehaviorTreeComponent->FindInstanceContainingNode(this))};
	FBTIsActorMovingDecoratorMemory* IsInRangeDecoratorMemory{CastNodeMemory(NodeMemory)};
	IsInRangeDecoratorMemory->TickInterval = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

FBTIsActorMovingDecoratorMemory* UBTDecorator_IsActorMoving::CastNodeMemory(uint8* NodeMemory) const
{
	return reinterpret_cast<FBTIsActorMovingDecoratorMemory*>(NodeMemory);
}

bool UBTDecorator_IsActorMoving::IsNearlyEqual(const FVector& First, const FVector& Second, float ErrorTolerance)
{
	const bool XIsEqual{FMath::IsNearlyEqual(First.X, Second.X, ErrorTolerance)};
	const bool YIsEqual{FMath::IsNearlyEqual(First.Y, Second.Y, ErrorTolerance)};
	const bool ZIsEqual{FMath::IsNearlyEqual(First.Z, Second.Z, ErrorTolerance)};

	return XIsEqual && YIsEqual && ZIsEqual;
}
