// Fill out your copyright notice in the Description page of Project Settings.


#include "IA/Decorators/BTDecorator_CheckVisibility.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_CheckVisibility::UBTDecorator_CheckVisibility()
{
	NodeName = "Is Actor Moving";

	bTickIntervals = true;
	bNotifyTick = true;
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void UBTDecorator_CheckVisibility::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTCheckVisibilityDecoratorMemory* CheckVisibilityDecoratorMemory{CastNodeMemory(NodeMemory)};
	
	if (CheckVisibilityDecoratorMemory->bInitialized)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};

	CheckVisibilityDecoratorMemory->TargetActor = Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(TargetActor.GetSelectedKeyID()));
	CheckVisibilityDecoratorMemory->TickInterval = TickInterval.GetValue(OwnerComp);
	CheckVisibilityDecoratorMemory->bInitialized = true;

	BlackboardComponent->RegisterObserver(TargetActor.GetSelectedKeyID(), this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_CheckVisibility::OnTargetActorKeyValueChange));

	const FBlackboard::FKey TickIntervalKey{TickInterval.GetKeyId(OwnerComp)};
	if (TickIntervalKey != FBlackboard::InvalidKey)
	{
		BlackboardComponent->RegisterObserver(TickIntervalKey, this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_CheckVisibility::OnTickIntervalKeyValueChange));
	}

	SetNextTickTime(NodeMemory, CheckVisibilityDecoratorMemory->TickInterval);
}

void UBTDecorator_CheckVisibility::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	BlackboardComponent->UnregisterObserversFrom(this);

	FBTCheckVisibilityDecoratorMemory* CheckVisibilityDecoratorMemory{CastNodeMemory(NodeMemory)};
	CheckVisibilityDecoratorMemory->LineTraceDidNotTouchedTheTarget = false;
	CheckVisibilityDecoratorMemory->bInitialized = false;
}

bool UBTDecorator_CheckVisibility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	const FBTCheckVisibilityDecoratorMemory* CheckVisibilityDecoratorMemory{CastNodeMemory(NodeMemory)};
	if (CheckVisibilityDecoratorMemory->LineTraceDidNotTouchedTheTarget)
	{
		return false;
	}
	
	const UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	const AActor* RetrievedTargetActor{Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(TargetActor.GetSelectedKeyID()))};
	
	if (!RetrievedTargetActor)
	{
		return false;
	}
	
	const AActor* TaskOwnerActor{OwnerComp.GetAIOwner()->GetPawn()};
	
	const FVector Start{TaskOwnerActor->GetActorLocation()};
	const FVector End{RetrievedTargetActor->GetActorLocation()};
	
	return DoLineTrace(Start, End, TaskOwnerActor, RetrievedTargetActor);
}

void UBTDecorator_CheckVisibility::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTCheckVisibilityDecoratorMemory* CheckVisibilityDecoratorMemory{CastNodeMemory(NodeMemory)};
	if (!CheckVisibilityDecoratorMemory->bInitialized)
	{
		return;
	}

	const AActor* RetrievedTargetActor{CheckVisibilityDecoratorMemory->TargetActor.Get()};

	if (!RetrievedTargetActor)
	{
		OwnerComp.RequestExecution(this);
		return;
	}
	
	const AActor* TaskOwnerActor{OwnerComp.GetAIOwner()->GetPawn()};
	
	const FVector Start{TaskOwnerActor->GetActorLocation()};
	const FVector End{RetrievedTargetActor->GetActorLocation()};
	
	if (!DoLineTrace(Start, End, TaskOwnerActor, RetrievedTargetActor))
	{
		CheckVisibilityDecoratorMemory->LineTraceDidNotTouchedTheTarget = true;
		OwnerComp.RequestExecution(this);
	}

	SetNextTickTime(NodeMemory, CheckVisibilityDecoratorMemory->TickInterval);
}

void UBTDecorator_CheckVisibility::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetActor.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		TargetActor.InvalidateResolvedKey();
	}
}

uint16 UBTDecorator_CheckVisibility::GetInstanceMemorySize() const
{
	return sizeof(FBTCheckVisibilityDecoratorMemory);
}

EBlackboardNotificationResult UBTDecorator_CheckVisibility::OnTickIntervalKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorTreeComponent{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	uint8* NodeMemory{BehaviorTreeComponent->GetNodeMemory(this, BehaviorTreeComponent->FindInstanceContainingNode(this))};
	FBTCheckVisibilityDecoratorMemory* CheckVisibilityDecoratorMemory{CastNodeMemory(NodeMemory)};
	CheckVisibilityDecoratorMemory->TickInterval = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTDecorator_CheckVisibility::OnTargetActorKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorTreeComponent{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	uint8* NodeMemory{BehaviorTreeComponent->GetNodeMemory(this, BehaviorTreeComponent->FindInstanceContainingNode(this))};
	FBTCheckVisibilityDecoratorMemory* CheckVisibilityDecoratorMemory{CastNodeMemory(NodeMemory)};
	CheckVisibilityDecoratorMemory->TargetActor = Cast<AActor>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));
	
	BehaviorTreeComponent->RequestExecution(this);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

FBTCheckVisibilityDecoratorMemory* UBTDecorator_CheckVisibility::CastNodeMemory(uint8* NodeMemory) const
{
	return reinterpret_cast<FBTCheckVisibilityDecoratorMemory*>(NodeMemory);
}

bool UBTDecorator_CheckVisibility::DoLineTrace(const FVector& Start, const FVector& End, const AActor* TaskOwnerActor, const AActor* RetrievedTargetActor) const
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(TaskOwnerActor);
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, CollisionChannel, CollisionQueryParams))
	{
		return false;
	}

	return HitResult.GetActor() == RetrievedTargetActor;
}
