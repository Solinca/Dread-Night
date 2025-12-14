#include "IA/Decorators/BTDecorator_IsActorMoving.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_IsActorMoving::UBTDecorator_IsActorMoving()
{
	NodeName = "Is Actor Moving";

	bTickIntervals = true;
	bNotifyTick = true;
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void UBTDecorator_IsActorMoving::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* ActorMovingDecoratorMemory{CastNodeMemory<FBTIsActorMovingDecoratorMemory>(NodeMemory)};
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

	RegisterToKeyIdChecked(TickInterval, OwnerComp, BlackboardComponent, this, &UBTDecorator_IsActorMoving::OnTickIntervalKeyValueChange);

	SetNextTickTime(NodeMemory, ActorMovingDecoratorMemory->TickInterval);
}

void UBTDecorator_IsActorMoving::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* ActorMovingDecoratorMemory{CastNodeMemory<FBTIsActorMovingDecoratorMemory>(NodeMemory)};
	ActorMovingDecoratorMemory->bHasMoved = false;
	ActorMovingDecoratorMemory->bInitialized = false;
	
	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	BlackboardComponent->UnregisterObserversFrom(this);
}

bool UBTDecorator_IsActorMoving::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto* ActorMovingDecoratorMemory{CastNodeMemory<FBTIsActorMovingDecoratorMemory>(NodeMemory)};

	const UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	const AActor* RetrievedActor{Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(Actor.GetSelectedKeyID()))};
	if (!RetrievedActor)
	{
		return false;
	}
	
	return ActorMovingDecoratorMemory->bHasMoved;
}

void UBTDecorator_IsActorMoving::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto* ActorMovingDecoratorMemory{CastNodeMemory<FBTIsActorMovingDecoratorMemory>(NodeMemory)};
	if (!ActorMovingDecoratorMemory->bInitialized)
	{
		return;
	}

	const FVector NewOldActorLocation{ActorMovingDecoratorMemory->Actor->GetActorLocation()};
	if (!IsNearlyEqual(NewOldActorLocation, ActorMovingDecoratorMemory->OldActorLocation))
	{
		ActorMovingDecoratorMemory->bHasMoved = true;
		OwnerComp.RequestExecution(this);
	}

	ActorMovingDecoratorMemory->OldActorLocation = NewOldActorLocation;

	SetNextTickTime(NodeMemory, ActorMovingDecoratorMemory->TickInterval);
}

void UBTDecorator_IsActorMoving::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		Actor.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		Actor.InvalidateResolvedKey();
	}
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
	auto* IsInRangeDecoratorMemory{CastNodeMemory<FBTIsActorMovingDecoratorMemory>(NodeMemory)};
	
	IsInRangeDecoratorMemory->Actor = Cast<AActor>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));

	BehaviorTreeComponent->RequestExecution(this);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTDecorator_IsActorMoving::OnTickIntervalKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* IsInRangeDecoratorMemory{CastNodeMemory<FBTIsActorMovingDecoratorMemory>(&Blackboard)};
	IsInRangeDecoratorMemory->TickInterval = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

bool UBTDecorator_IsActorMoving::IsNearlyEqual(const FVector& First, const FVector& Second, const float ErrorTolerance)
{
	const bool XIsEqual{FMath::IsNearlyEqual(First.X, Second.X, ErrorTolerance)};
	const bool YIsEqual{FMath::IsNearlyEqual(First.Y, Second.Y, ErrorTolerance)};
	const bool ZIsEqual{FMath::IsNearlyEqual(First.Z, Second.Z, ErrorTolerance)};

	return XIsEqual && YIsEqual && ZIsEqual;
}
