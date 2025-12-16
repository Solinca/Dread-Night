#include "IA/Tasks/BTTask_RangeAttack.h"

#include "AIController.h"
#include "Actors/ProjectileActor.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "DamageSystem/Interface/Damageable.h"

UBTTask_RangeAttack::UBTTask_RangeAttack()
{
	NodeName = "Range Attack";

	INIT_TASK_NODE_NOTIFY_FLAGS();

	bTickIntervals = true;

	AttackedTarget.AllowNoneAsValue(false);
	AttackedTarget.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RangeAttack, AttackedTarget), AActor::StaticClass());
	//For now the base class will be equal to this Projectile Actor class, and later I will see if it needs to be changed.
	SpawnedProjectile.SetBaseClass(AProjectileActor::StaticClass());
}

EBTNodeResult::Type UBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* RangeAttackTaskMemory{CastNodeMemory<FBTRangeAttackTaskMemory>(NodeMemory)};
	if (RangeAttackTaskMemory->bInitialized)
	{
		return EBTNodeResult::Succeeded;
	}

	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};

	RangeAttackTaskMemory->AttackCooldown = AttackCooldown.GetValue(OwnerComp);
	RangeAttackTaskMemory->AttackDamage = AttackDamage.GetValue(OwnerComp);
	RangeAttackTaskMemory->AttackedTarget = Cast<AActor>(BlackboardComponent->GetValue<UBlackboardKeyType_Object>(AttackedTarget.GetSelectedKeyID()));
	RangeAttackTaskMemory->SpawnedProjectile = SpawnedProjectile.GetValue(OwnerComp);
	
	if (!RangeAttackTaskMemory->AttackedTarget.IsValid() ||
		!RangeAttackTaskMemory->AttackedTarget->Implements<UDamageable>() ||
		!RangeAttackTaskMemory->SpawnedProjectile)
	{
		return EBTNodeResult::Failed;
	}
	
	RegisterToKeyIdChecked(SpawnedProjectile, OwnerComp, BlackboardComponent, this, &UBTTask_RangeAttack::OnSpawnedProjectileKeyValueChange);
	RegisterToKeyIdChecked(AttackCooldown, OwnerComp, BlackboardComponent, this, &UBTTask_RangeAttack::OnAttackCooldownKeyValueChange);
	RegisterToKeyIdChecked(AttackDamage, OwnerComp, BlackboardComponent, this, &UBTTask_RangeAttack::OnAttackDamageKeyValueChange);

	BlackboardComponent->RegisterObserver(AttackedTarget.GetSelectedKeyID(), this,
		FOnBlackboardChangeNotification::CreateUObject(this, &UBTTask_RangeAttack::OnAttackedTargetKeyValueChange));

	AAIController* AIController{OwnerComp.GetAIOwner()};
	AIController->SetFocus(RangeAttackTaskMemory->AttackedTarget.Get());

	RangeAttackTaskMemory->bInitialized = true;

	SetNextTickTime(NodeMemory, RangeAttackTaskMemory->AttackCooldown);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_RangeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto* RangeAttackTaskMemory{CastNodeMemory<FBTRangeAttackTaskMemory>(NodeMemory)};
	if (!RangeAttackTaskMemory->bInitialized || !RangeAttackTaskMemory->SpawnedProjectile)
	{
		FinishLatentAbort(OwnerComp);
		return;
	}

	APawn* SpawnInstigator{OwnerComp.GetAIOwner()->GetPawn()};
	IDamageable* Damageable{Cast<IDamageable>(RangeAttackTaskMemory->AttackedTarget.Get())};
	
	if (!Damageable || !SpawnInstigator)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FTransform ProjectileTransform;
	ProjectileTransform.SetScale3D(FVector::One());
	ProjectileTransform.SetLocation(SpawnInstigator->GetActorLocation());
	ProjectileTransform.SetRotation(SpawnInstigator->GetActorRotation().Quaternion());
	
	AProjectileActor* ProjectileActor{
		GetWorld()->SpawnActorDeferred<AProjectileActor>(
			RangeAttackTaskMemory->SpawnedProjectile,
			ProjectileTransform,
			nullptr,
			SpawnInstigator,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		)
	};

	if (ProjectileActor)
	{
		UProjectileMovementComponent* ProjectileMovementComponent{ProjectileActor->GetProjectileMovementComponent()};
		
		ProjectileActor->SetDamage(RangeAttackTaskMemory->AttackDamage);
		ProjectileActor->FinishSpawning(ProjectileTransform);

		const AActor* RetrievedActor{RangeAttackTaskMemory->AttackedTarget.Get()};
		const FVector ProjectileVelocity{(RetrievedActor->GetActorLocation() - SpawnInstigator->GetActorLocation()).GetSafeNormal() * ProjectileMovementComponent->InitialSpeed};
		ProjectileMovementComponent->Velocity = ProjectileVelocity;
	}

	SetNextTickTime(NodeMemory, RangeAttackTaskMemory->AttackCooldown);
}

void UBTTask_RangeAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
										EBTNodeResult::Type TaskResult)
{
	auto* AttackTaskMemory{CastNodeMemory<FBTRangeAttackTaskMemory>(NodeMemory)};
	AttackTaskMemory->bInitialized = false;
	AttackTaskMemory->AttackedTarget.Reset();

	UBlackboardComponent* BlackboardComponent{OwnerComp.GetBlackboardComponent()};
	BlackboardComponent->UnregisterObserversFrom(this);

	AAIController* AIController{OwnerComp.GetAIOwner()};
	AIController->ClearFocus(EAIFocusPriority::LastFocusPriority);
}

void UBTTask_RangeAttack::InitializeFromAsset(UBehaviorTree& Asset)
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

uint16 UBTTask_RangeAttack::GetInstanceMemorySize() const
{
	return sizeof(FBTRangeAttackTaskMemory);
}

EBlackboardNotificationResult UBTTask_RangeAttack::OnSpawnedProjectileKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{CastNodeMemory<FBTRangeAttackTaskMemory>(&Blackboard)};
	AttackTaskMemory->SpawnedProjectile = Blackboard.GetValue<UBlackboardKeyType_Class>(ChangedKeyID);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_RangeAttack::OnAttackCooldownKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{CastNodeMemory<FBTRangeAttackTaskMemory>(&Blackboard)};
	AttackTaskMemory->AttackCooldown = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_RangeAttack::OnAttackDamageKeyValueChange(const UBlackboardComponent& Blackboard,
	FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{CastNodeMemory<FBTRangeAttackTaskMemory>(&Blackboard)};
	AttackTaskMemory->AttackDamage = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);
	
	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_RangeAttack::OnAttackedTargetKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{CastNodeMemory<FBTRangeAttackTaskMemory>(&Blackboard)};
	AttackTaskMemory->AttackedTarget = Cast<AActor>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));
	
	return EBlackboardNotificationResult::ContinueObserving;
}
