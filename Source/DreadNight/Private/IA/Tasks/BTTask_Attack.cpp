#include "IA/Tasks/BTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "DamageSystem/Interface/Damageable.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack";
	
	INIT_TASK_NODE_NOTIFY_FLAGS();

	bTickIntervals = true;

	AttackedTarget.AllowNoneAsValue(false);
	AttackedTarget.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Attack, AttackedTarget), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTAttackTaskMemory* AttackTaskMemory{CastNodeMemory<FBTAttackTaskMemory>(NodeMemory)};
	if (AttackTaskMemory->bInitialized)
	{
		return EBTNodeResult::Succeeded;
	}

	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	
	AttackTaskMemory->AttackCooldown = AttackCooldown.GetValue(OwnerComp);
	AttackTaskMemory->AttackDamage = AttackDamage.GetValue(OwnerComp);
	AttackTaskMemory->AttackedTarget = Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(AttackedTarget.GetSelectedKeyID()));

	if (!AttackTaskMemory->AttackedTarget.IsValid() ||
		!AttackTaskMemory->AttackedTarget->Implements<UDamageable>())
	{
		return EBTNodeResult::Failed;
	}
	
	RegisterToKeyIdChecked(AttackCooldown, OwnerComp, BlackboardComponent, this, &UBTTask_Attack::OnAttackCooldownKeyValueChange);
	RegisterToKeyIdChecked(AttackDamage, OwnerComp, BlackboardComponent, this, &UBTTask_Attack::OnAttackDamageKeyValueChange);

	BlackboardComponent->RegisterObserver(AttackedTarget.GetSelectedKeyID(), this,
		FOnBlackboardChangeNotification::CreateUObject(this, &UBTTask_Attack::OnAttackedTargetKeyValueChange));

	AAIController* AIController{OwnerComp.GetAIOwner()};
	AIController->SetFocus(AttackTaskMemory->AttackedTarget.Get());
	
	AttackTaskMemory->bInitialized = true;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto* AttackTaskMemory{CastNodeMemory<FBTAttackTaskMemory>(NodeMemory)};
	if (!AttackTaskMemory->bInitialized)
	{
		FinishLatentAbort(OwnerComp);
		return;
	}

	AActor* DamageInstigator{OwnerComp.GetAIOwner()->GetPawn()};
	IDamageable* Damageable{Cast<IDamageable>(AttackTaskMemory->AttackedTarget.Get())};
	
	if (!Damageable || !DamageInstigator)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
	Damageable->TryApplyDamage(AttackTaskMemory->AttackDamage, DamageInstigator);
	
	SetNextTickTime(NodeMemory, AttackTaskMemory->AttackCooldown);
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
									EBTNodeResult::Type TaskResult)
{
	auto* AttackTaskMemory{CastNodeMemory<FBTAttackTaskMemory>(NodeMemory)};
	AttackTaskMemory->bInitialized = false;
	AttackTaskMemory->AttackedTarget.Reset();

	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	BlackboardComponent->UnregisterObserversFrom(this);

	AAIController* AIController{OwnerComp.GetAIOwner()};
	AIController->ClearFocus(EAIFocusPriority::LastFocusPriority);
}

void UBTTask_Attack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		AttackedTarget.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		AttackedTarget.InvalidateResolvedKey();
	}
}

uint16 UBTTask_Attack::GetInstanceMemorySize() const
{
	return sizeof(FBTAttackTaskMemory);
}

EBlackboardNotificationResult UBTTask_Attack::OnAttackedTargetKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{CastNodeMemory<FBTAttackTaskMemory>(&Blackboard)};
	AttackTaskMemory->AttackedTarget = Cast<AActor>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));
	
	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_Attack::OnAttackCooldownKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{CastNodeMemory<FBTAttackTaskMemory>(&Blackboard)};
	AttackTaskMemory->AttackCooldown = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_Attack::OnAttackDamageKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{CastNodeMemory<FBTAttackTaskMemory>(&Blackboard)};
	AttackTaskMemory->AttackDamage = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);
	
	return EBlackboardNotificationResult::ContinueObserving;
}
