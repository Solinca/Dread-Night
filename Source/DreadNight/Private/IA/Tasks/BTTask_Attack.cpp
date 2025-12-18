#include "IA/Tasks/BTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "DamageSystem/Interface/Damageable.h"
#include "GameFramework/Character.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack";

	INIT_TASK_NODE_NOTIFY_FLAGS();

	bTickIntervals = true;

	AttackedTarget.AllowNoneAsValue(false);
	AttackedTarget.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Attack, AttackedTarget),
	                               AActor::StaticClass());

	AttackAnimationMontage.SetBaseClass(UAnimMontage::StaticClass());
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTAttackTaskMemory* AttackTaskMemory{ CastNodeMemory<FBTAttackTaskMemory>(NodeMemory) };
	if (AttackTaskMemory->bInitialized)
	{
		return EBTNodeResult::Succeeded;
	}

	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };

	AttackTaskMemory->AttackCooldown = AttackCooldown.GetValue(OwnerComp);
	AttackTaskMemory->AttackDamage = AttackDamage.GetValue(OwnerComp);
	AttackTaskMemory->AttackedTarget = Cast<AActor>(
		BlackboardComponent->GetValue<UBlackboardKeyType_Object>(AttackedTarget.GetSelectedKeyID()));
	AttackTaskMemory->AttackAnimationMontage = AttackAnimationMontage.GetValue<UAnimMontage>(OwnerComp);
	AttackTaskMemory->AnimInstance = OwnerComp.GetAIOwner()->GetCharacter()->GetMesh()->GetAnimInstance();

	AttackTaskMemory->AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(
		this, &UBTTask_Attack::OnAttackNotifyBegin);

	if (!AttackTaskMemory->AttackedTarget.IsValid() ||
		!AttackTaskMemory->AttackedTarget->Implements<UDamageable>() || !AttackTaskMemory->AttackAnimationMontage.
		IsValid())
	{
		return EBTNodeResult::Failed;
	}

	RegisterToKeyIdChecked(AttackCooldown, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_Attack::OnAttackCooldownKeyValueChange);
	RegisterToKeyIdChecked(AttackDamage, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_Attack::OnAttackDamageKeyValueChange);

	BlackboardComponent->RegisterObserver(AttackedTarget.GetSelectedKeyID(), this,
	                                      FOnBlackboardChangeNotification::CreateUObject(
		                                      this, &UBTTask_Attack::OnAttackedTargetKeyValueChange));

	AAIController* AIController{ OwnerComp.GetAIOwner() };
	AIController->SetFocus(AttackTaskMemory->AttackedTarget.Get());

	AttackTaskMemory->bInitialized = true;

	constexpr float StartingRate{ 0.1f };
	LaunchMontageTimer(AIController->GetCharacter(), AttackTaskMemory, &OwnerComp, StartingRate);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                    EBTNodeResult::Type TaskResult)
{
	auto* AttackTaskMemory{ CastNodeMemory<FBTAttackTaskMemory>(NodeMemory) };
	AttackTaskMemory->bInitialized = false;
	AttackTaskMemory->AttackedTarget.Reset();

	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	BlackboardComponent->UnregisterObserversFrom(this);

	AAIController* AIController{ OwnerComp.GetAIOwner() };
	AIController->ClearFocus(EAIFocusPriority::LastFocusPriority);

	AttackTaskMemory->AnimInstance->OnPlayMontageNotifyBegin.RemoveAll(this);

	//Need to check this because MontageEnded can be equal to nullptr.
	if (FOnMontageEnded* MontageEnded = AttackTaskMemory->AnimInstance->Montage_GetEndedDelegate(
		AttackTaskMemory->AttackAnimationMontage.Get()))
	{
		MontageEnded->Unbind();
	}

	//Then do a Blend out time to avoid cutting the animation and instead does a smooth transition.
	AttackTaskMemory->AnimInstance->Montage_Stop(0.25f);

	GetWorld()->GetTimerManager().ClearTimer(AttackTaskMemory->PlayMontageHandle);
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

void UBTTask_Attack::LaunchMontageTimer(ACharacter* AICharacter, FBTAttackTaskMemory* AttackTaskMemory,
                                        UBehaviorTreeComponent* BehaviorTreeComponent, const float Rate)
{
	//Assign a lambda to have access to RangeAttackTaskMemory, BehaviorTreeComponent and AICharacter.
	GetWorld()->GetTimerManager().SetTimer(AttackTaskMemory->PlayMontageHandle,
       [this, AICharacter, AttackTaskMemory, BehaviorTreeComponent]
       {
           OnPlayMontageTimerFinish(AICharacter, AttackTaskMemory, BehaviorTreeComponent);
       }, Rate, false);
}

void UBTTask_Attack::OnAttackNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName != TEXT("Attack"))
	{
		return;
	}

	//Pretty much horrible, but it avoids me to do an AI Task just for this animation callback.
	ACharacter* DamageInstigator{ Cast<ACharacter>(BranchingPointPayload.SkelMeshComponent->GetOwner()) };
	const AAIController* AIController{ Cast<AAIController>(DamageInstigator->GetController()) };
	UBehaviorTreeComponent* BehaviorTreeComponent{ Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()) };
	const FBTAttackTaskMemory* AttackTaskMemory{
		CastNodeMemory<FBTAttackTaskMemory>(AIController->GetBlackboardComponent())
	};

	if (!AttackTaskMemory->bInitialized)
	{
		FinishLatentAbort(*BehaviorTreeComponent);
		return;
	}

	IDamageable* Damageable{ Cast<IDamageable>(AttackTaskMemory->AttackedTarget.Get()) };

	if (!Damageable || !DamageInstigator)
	{
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
		return;
	}
	
	Damageable->TryApplyDamage(AttackTaskMemory->AttackDamage, DamageInstigator);
}

void UBTTask_Attack::OnPlayMontageTimerFinish(ACharacter* AICharacter, FBTAttackTaskMemory* AttackTaskMemory,
                                              UBehaviorTreeComponent* BehaviorTreeComponent)
{
	if (!AICharacter || !AttackTaskMemory)
	{
		FinishLatentAbort(*BehaviorTreeComponent);
		return;
	}

	AICharacter->PlayAnimMontage(AttackTaskMemory->AttackAnimationMontage.Get());

	BindEndAnimEvent(AttackTaskMemory, BehaviorTreeComponent);
}

void UBTTask_Attack::BindEndAnimEvent(FBTAttackTaskMemory* AttackTaskMemory,
                                      UBehaviorTreeComponent* BehaviorTreeComponent)
{
	//Assign a lambda to get access to the task memory and behavior tree component.
	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindLambda([this, AttackTaskMemory, BehaviorTreeComponent]
	(UAnimMontage* Montage, bool bInterrupted)
		{
			OnEndMontage(AttackTaskMemory, BehaviorTreeComponent, Montage, bInterrupted);
		});

	AttackTaskMemory->AnimInstance->Montage_SetEndDelegate(OnMontageEnded,
	                                                       AttackTaskMemory->AttackAnimationMontage.Get());
}

void UBTTask_Attack::OnEndMontage(FBTAttackTaskMemory* AttackTaskMemory, UBehaviorTreeComponent* OwnerComp,
                                  UAnimMontage* Montage, bool bInterrupted)
{
	if (!AttackTaskMemory->bInitialized)
	{
		FinishLatentAbort(*OwnerComp);
		return;
	}

	ACharacter* AICharacter{ OwnerComp->GetAIOwner()->GetCharacter() };
	IDamageable* Damageable{ Cast<IDamageable>(AttackTaskMemory->AttackedTarget.Get()) };

	if (!Damageable || !AICharacter)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	LaunchMontageTimer(AICharacter, AttackTaskMemory, OwnerComp, AttackTaskMemory->AttackCooldown);
}

uint16 UBTTask_Attack::GetInstanceMemorySize() const
{
	return sizeof(FBTAttackTaskMemory);
}

EBlackboardNotificationResult UBTTask_Attack::OnAttackedTargetKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{ CastNodeMemory<FBTAttackTaskMemory>(&Blackboard) };
	AttackTaskMemory->AttackedTarget = Cast<AActor>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_Attack::OnAttackCooldownKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{ CastNodeMemory<FBTAttackTaskMemory>(&Blackboard) };
	AttackTaskMemory->AttackCooldown = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_Attack::OnAttackDamageKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{ CastNodeMemory<FBTAttackTaskMemory>(&Blackboard) };
	AttackTaskMemory->AttackDamage = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);

	return EBlackboardNotificationResult::ContinueObserving;
}
