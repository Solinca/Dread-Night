#include "IA/Decorators/BTDecorator_IsInRange.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameFramework/Character.h"

UBTDecorator_IsInRange::UBTDecorator_IsInRange()
{
	NodeName = "Is In Range";

	TargetActor.AllowNoneAsValue(false);
	TargetActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInRange, TargetActor), AActor::StaticClass());

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bNotifyTick = true;
}

void UBTDecorator_IsInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTIsInRangeDecoratorMemory* CastedNodeMemory{CastNodeMemory(NodeMemory)};
	if (CastedNodeMemory->bInitialized)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
		
	CastedNodeMemory->TargetActor = Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(TargetActor.GetSelectedKeyID()));
	CastedNodeMemory->Range = Range.GetValue(OwnerComp);
	CastedNodeMemory->bInitialized = true;

	BlackboardComponent->RegisterObserver(TargetActor.GetSelectedKeyID(), this,
		FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_IsInRange::OnTargetActorKeyValueChange));

	const FBlackboard::FKey RangeKey{Range.GetKeyId(OwnerComp)};
	if (RangeKey == FBlackboard::InvalidKey)
	{
		return;
	}

	BlackboardComponent->RegisterObserver(RangeKey, this,
		FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_IsInRange::OnRangeKeyValueChange));
}

void UBTDecorator_IsInRange::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTIsInRangeDecoratorMemory* CastedNodeMemory{CastNodeMemory(NodeMemory)};
	CastedNodeMemory->bInitialized = false;
	
	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	BlackboardComponent->UnregisterObserversFrom(this);
}

bool UBTDecorator_IsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (!OwnerComp.GetAIOwner() || !OwnerComp.GetAIOwner()->GetCharacter())
	{
		return false;
	}
	
	const UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	const AActor* RetrievedTargetActor = Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(TargetActor.GetSelectedKeyID()));
	
	if (!RetrievedTargetActor)
	{
		return false;
	}

	const float RetrievedRange = Range.GetValue(OwnerComp);
	
	const AActor* OwnerActor{OwnerComp.GetAIOwner()->GetCharacter()};
	return OwnerActor->GetDistanceTo(RetrievedTargetActor) <= RetrievedRange;
}

void UBTDecorator_IsInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const FBTIsInRangeDecoratorMemory* CastedNodeMemory{CastNodeMemory(NodeMemory)};
	if (!CastedNodeMemory->bInitialized)
	{
		return;
	}

	const AActor* OwnerActor{OwnerComp.GetAIOwner()->GetPawn()};
	const AActor* RetrievedTargetActor{CastedNodeMemory->TargetActor.Get()};

	if (!RetrievedTargetActor || !OwnerActor)
	{
		OwnerComp.RequestExecution(this);
	}
	
	const bool bIsInRange{OwnerActor->GetDistanceTo(RetrievedTargetActor) <= CastedNodeMemory->Range};
	const bool bShouldBeActive{IsInversed() ? !bIsInRange : bIsInRange};

	if (!bShouldBeActive)
	{
		OwnerComp.RequestExecution(this);
	}
}

void UBTDecorator_IsInRange::InitializeFromAsset(UBehaviorTree& Asset)
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

uint16 UBTDecorator_IsInRange::GetInstanceMemorySize() const
{
	return sizeof(FBTIsInRangeDecoratorMemory);
}

EBlackboardNotificationResult UBTDecorator_IsInRange::OnTargetActorKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorTreeComponent{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	uint8* NodeMemory{BehaviorTreeComponent->GetNodeMemory(this, BehaviorTreeComponent->FindInstanceContainingNode(this))};
	FBTIsInRangeDecoratorMemory* IsInRangeDecoratorMemory{CastNodeMemory(NodeMemory)};
	IsInRangeDecoratorMemory->TargetActor = Cast<AActor>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));
	
	BehaviorTreeComponent->RequestExecution(this);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTDecorator_IsInRange::OnRangeKeyValueChange(const UBlackboardComponent& Blackboard,
	FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorTreeComponent{Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent())};
	uint8* NodeMemory{BehaviorTreeComponent->GetNodeMemory(this, BehaviorTreeComponent->FindInstanceContainingNode(this))};
	FBTIsInRangeDecoratorMemory* IsInRangeDecoratorMemory{CastNodeMemory(NodeMemory)};
	IsInRangeDecoratorMemory->Range = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);
	
	BehaviorTreeComponent->RequestExecution(this);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

FBTIsInRangeDecoratorMemory* UBTDecorator_IsInRange::CastNodeMemory(uint8* NodeMemory) const
{
	return reinterpret_cast<FBTIsInRangeDecoratorMemory*>(NodeMemory);
}
