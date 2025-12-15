#include "IA/Tasks/BTTask_Wander.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQuery.h"

#define IS_VALID(Value) Value ? TEXT("True") : TEXT("False")

void FBTWanderTaskMemory::CleanMemory()
{
	Task.Reset();
	WanderQuery.Reset();
	bIsInitialized = false;
}

UBTTask_Wander::UBTTask_Wander(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	NodeName = "Wander";

	// Ensures OnTaskFinished is called.
	bNotifyTaskFinished = true;

	// Configure blackboard key filters.
	WanderStopKey.AllowNoneAsValue(false);
	WanderStopKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Wander, WanderStopKey));
	WanderStopKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Wander, WanderStopKey), AActor::StaticClass());

	WanderQuery.SetBaseClass(UEnvQuery::StaticClass());
}

EBTNodeResult::Type UBTTask_Wander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* WanderNodeMemory{ CastNodeMemory<FBTWanderTaskMemory>(NodeMemory) };
	if (!WanderNodeMemory)
	{
		//TODO : Log error.
		return EBTNodeResult::Failed;
	}

	AAIController* AIController{ OwnerComp.GetAIOwner() };

	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	
	WanderNodeMemory->WanderQuery = WanderQuery.GetValue<UEnvQuery>(OwnerComp);
	WanderNodeMemory->AcceptableRadius = AcceptableRadius.GetValue(OwnerComp);
	WanderNodeMemory->WanderWaitTime = WanderWaitTime.GetValue(OwnerComp);
	WanderNodeMemory->bInvertedCondition = InvertedCondition.GetValue(OwnerComp);
	WanderNodeMemory->bIsInitialized = true;

	if (WanderStopKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		WanderNodeMemory->WanderStop.Emplace<UObject*>(
			BlackboardComponent->GetValueAsObject(WanderStopKey.SelectedKeyName));
	}
	else if (WanderStopKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		WanderNodeMemory->WanderStop.Emplace<
			bool>(BlackboardComponent->GetValueAsBool(WanderStopKey.SelectedKeyName));
	}

	if (!WanderNodeMemory->WanderQuery.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UBTTask_Wander: The wander query is not set."))
		return EBTNodeResult::Failed;
	}

	if (HasToStop(NodeMemory))
	{
		return EBTNodeResult::Succeeded;
	}

	RegisterToKeyIdChecked(WanderQuery, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_Wander::OnWanderQueryValueChanged);
	RegisterToKeyIdChecked(AcceptableRadius, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_Wander::OnAcceptableRadiusValueChanged);
	RegisterToKeyIdChecked(WanderWaitTime, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_Wander::OnWanderWaitTimeValueChanged);
	RegisterToKeyIdChecked(InvertedCondition, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_Wander::OnInvertedConditionValueChanged);

	BlackboardComponent->RegisterObserver(WanderStopKey.GetSelectedKeyID(), this,
	                                      FOnBlackboardChangeNotification::CreateUObject(
		                                      this, &UBTTask_Wander::OnWanderStopValueChanged));

	const FAiTaskWanderParameters AiTaskWanderParameters{WanderNodeMemory->AcceptableRadius, WanderNodeMemory->WanderWaitTime, WanderNodeMemory->WanderQuery.Get()};
	UAITask_Wander* NewTask = UAITask_Wander::Create(*AIController, *this, AiTaskWanderParameters);

	if (!NewTask)
	{
		//TODO : Log error.
		return EBTNodeResult::Failed;
	}

	WanderNodeMemory->Task = NewTask;
	NewTask->ReadyForActivation();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Wander::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* WanderTaskMemory{ CastNodeMemory<FBTWanderTaskMemory>(NodeMemory) };

	if (WanderTaskMemory->Task.IsValid())
	{
		WanderTaskMemory->Task->ExternalCancel();
	}

	return EBTNodeResult::Aborted;
}

void UBTTask_Wander::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	const UAITask_Wander* WanderTask{ Cast<UAITask_Wander>(&Task) };

	UBehaviorTreeComponent* BehaviorTreeComponent{ GetBTComponentForTask(Task) };
	ensure(BehaviorTreeComponent);

	if (AAIController* AiOwner = BehaviorTreeComponent->GetAIOwner())
	{
		if (UBlackboardComponent* BlackboardComponent = AiOwner->GetBlackboardComponent())
		{
			BlackboardComponent->UnregisterObserversFrom(this);
		}
	}

	FinishLatentTask(*BehaviorTreeComponent,
	                 WanderTask->HasFailed() ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
}

void UBTTask_Wander::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                    EBTNodeResult::Type TaskResult)
{
	auto* WanderTaskMemory{ CastNodeMemory<FBTWanderTaskMemory>(NodeMemory) };
	WanderTaskMemory->CleanMemory();
}

void UBTTask_Wander::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		WanderStopKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		WanderStopKey.InvalidateResolvedKey();
	}
}

uint16 UBTTask_Wander::GetInstanceMemorySize() const
{
	return sizeof(FBTWanderTaskMemory);
}

bool UBTTask_Wander::HasToStop(uint8* NodeMemory)
{
	auto* WanderTaskMemory{ CastNodeMemory<FBTWanderTaskMemory>(NodeMemory) };

	if (auto* WanderStopValue = WanderTaskMemory->WanderStop.TryGet<UObject*>())
	{
		return WanderTaskMemory->bInvertedCondition ? !IsValid(*WanderStopValue) : IsValid(*WanderStopValue);
	}

	if (auto* WanderStopValue = WanderTaskMemory->WanderStop.TryGet<bool>())
	{
		return WanderTaskMemory->bInvertedCondition ? !*WanderStopValue : *WanderStopValue;
	}

	return false;
}

EBlackboardNotificationResult UBTTask_Wander::OnWanderQueryValueChanged(const UBlackboardComponent& BlackboardComponent,
                                                                        FBlackboard::FKey keyID)
{
	auto* WanderTaskMemory{ CastNodeMemory<FBTWanderTaskMemory>(&BlackboardComponent) };
	WanderTaskMemory->WanderQuery = Cast<UEnvQuery>(WanderQuery.GetValue(BlackboardComponent));
	WanderTaskMemory->Task->GetAiTaskParameters().WanderQuery = WanderTaskMemory->WanderQuery.Get();
	WanderTaskMemory->Task->Restart();

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_Wander::OnAcceptableRadiusValueChanged(
	const UBlackboardComponent& BlackboardComponent, FBlackboard::FKey keyID)
{
	auto* WanderTaskMemory{ CastNodeMemory<FBTWanderTaskMemory>(&BlackboardComponent) };
	WanderTaskMemory->AcceptableRadius = AcceptableRadius.GetValue(BlackboardComponent);
	WanderTaskMemory->Task->GetAiTaskParameters().AcceptableRadius = WanderTaskMemory->AcceptableRadius;

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_Wander::OnWanderStopValueChanged(const UBlackboardComponent& BlackboardComponent,
                                                                       FBlackboard::FKey keyID)
{
	auto* WanderTaskMemory{ CastNodeMemory<FBTWanderTaskMemory>(&BlackboardComponent) };

	if (WanderStopKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		WanderTaskMemory->WanderStop.Emplace<UObject*>(
			BlackboardComponent.GetValueAsObject(WanderStopKey.SelectedKeyName));
	}
	else if (WanderStopKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		WanderTaskMemory->WanderStop.Emplace<
			bool>(BlackboardComponent.GetValueAsBool(WanderStopKey.SelectedKeyName));
	}
	
	if (!HasToStop(reinterpret_cast<uint8*>(WanderTaskMemory)))
	{
		return EBlackboardNotificationResult::ContinueObserving;
	}

	if (WanderTaskMemory->Task.IsValid())
	{
		WanderTaskMemory->Task->ExternalCancel();
	}

	return EBlackboardNotificationResult::RemoveObserver;
}

EBlackboardNotificationResult UBTTask_Wander::OnInvertedConditionValueChanged(
	const UBlackboardComponent& BlackboardComponent, FBlackboard::FKey keyID)
{
	auto* WanderTaskMemory{ CastNodeMemory<FBTWanderTaskMemory>(&BlackboardComponent) };
	WanderTaskMemory->bInvertedCondition = InvertedCondition.GetValue(BlackboardComponent);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_Wander::OnWanderWaitTimeValueChanged(
	const UBlackboardComponent& BlackboardComponent, FBlackboard::FKey keyID)
{
	auto* WanderTaskMemory{ CastNodeMemory<FBTWanderTaskMemory>(&BlackboardComponent) };
	WanderTaskMemory->WanderWaitTime = WanderWaitTime.GetValue(BlackboardComponent);
	WanderTaskMemory->Task->GetAiTaskParameters().WanderWaitTime = WanderTaskMemory->WanderWaitTime;
	
	return EBlackboardNotificationResult::ContinueObserving;
}
