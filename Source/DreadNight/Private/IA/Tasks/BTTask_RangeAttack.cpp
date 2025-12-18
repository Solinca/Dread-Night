#include "IA/Tasks/BTTask_RangeAttack.h"

#include "AIController.h"
#include "Actors/ProjectileActor.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "DamageSystem/Interface/Damageable.h"
#include "GameFramework/Character.h"

UBTTask_RangeAttack::UBTTask_RangeAttack()
{
	NodeName = "Range Attack";

	bNotifyTaskFinished = true;

	AttackedTarget.AllowNoneAsValue(false);
	AttackedTarget.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RangeAttack, AttackedTarget),
	                               AActor::StaticClass());
	//For now the base class will be equal to this Projectile Actor class, and later I will see if it needs to be changed.
	SpawnedProjectile.SetBaseClass(AProjectileActor::StaticClass());
}

EBTNodeResult::Type UBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* RangeAttackTaskMemory{ CastNodeMemory<FBTRangeAttackTaskMemory>(NodeMemory) };
	if (RangeAttackTaskMemory->bInitialized)
	{
		return EBTNodeResult::Succeeded;
	}

	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };

	RangeAttackTaskMemory->AttackCooldown = AttackCooldown.GetValue(OwnerComp);
	RangeAttackTaskMemory->AttackDamage = AttackDamage.GetValue(OwnerComp);
	RangeAttackTaskMemory->AttackedTarget = Cast<AActor>(
		BlackboardComponent->GetValue<UBlackboardKeyType_Object>(AttackedTarget.GetSelectedKeyID()));
	RangeAttackTaskMemory->SpawnedProjectile = SpawnedProjectile.GetValue(OwnerComp);
	RangeAttackTaskMemory->AttackAnimationMontage = AttackAnimationMontage.GetValue<UAnimMontage>(OwnerComp);
	RangeAttackTaskMemory->AnimInstance = OwnerComp.GetAIOwner()->GetCharacter()->GetMesh()->GetAnimInstance();
	RangeAttackTaskMemory->ThrowingBoneName = ThrowingBoneName.GetValue(OwnerComp);

	RangeAttackTaskMemory->AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(
		this, &UBTTask_RangeAttack::OnAttackNotifyBegin);

	if (!RangeAttackTaskMemory->AttackedTarget.IsValid() ||
		!RangeAttackTaskMemory->AttackedTarget->Implements<UDamageable>() ||
		!RangeAttackTaskMemory->SpawnedProjectile)
	{
		return EBTNodeResult::Failed;
	}

	RegisterToKeyIdChecked(SpawnedProjectile, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_RangeAttack::OnSpawnedProjectileKeyValueChange);
	RegisterToKeyIdChecked(AttackCooldown, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_RangeAttack::OnAttackCooldownKeyValueChange);
	RegisterToKeyIdChecked(AttackDamage, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_RangeAttack::OnAttackDamageKeyValueChange);

	BlackboardComponent->RegisterObserver(AttackedTarget.GetSelectedKeyID(), this,
	                                      FOnBlackboardChangeNotification::CreateUObject(
		                                      this, &UBTTask_RangeAttack::OnAttackedTargetKeyValueChange));

	AAIController* AIController{ OwnerComp.GetAIOwner() };
	AIController->SetFocus(RangeAttackTaskMemory->AttackedTarget.Get());

	RangeAttackTaskMemory->bInitialized = true;

	constexpr float StartingRate{ 0.1f };
	LaunchMontageTimer(AIController->GetCharacter(), RangeAttackTaskMemory, &OwnerComp, StartingRate);

	return EBTNodeResult::InProgress;
}

void UBTTask_RangeAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                         EBTNodeResult::Type TaskResult)
{
	auto* RangeAttackTaskMemory{ CastNodeMemory<FBTRangeAttackTaskMemory>(NodeMemory) };
	RangeAttackTaskMemory->bInitialized = false;
	RangeAttackTaskMemory->AttackedTarget.Reset();

	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	BlackboardComponent->UnregisterObserversFrom(this);

	AAIController* AIController{ OwnerComp.GetAIOwner() };
	AIController->ClearFocus(EAIFocusPriority::LastFocusPriority);

	RangeAttackTaskMemory->AnimInstance->OnPlayMontageNotifyBegin.RemoveAll(this);

	//Need to check this because MontageEnded can be equal to nullptr.
	if (FOnMontageEnded* MontageEnded = RangeAttackTaskMemory->AnimInstance->Montage_GetEndedDelegate(
		RangeAttackTaskMemory->AttackAnimationMontage.Get()))
	{
		MontageEnded->Unbind();
	}

	//Then do a Blend out time to avoid cutting the animation and instead does a smooth transition.
	RangeAttackTaskMemory->AnimInstance->Montage_Stop(0.25f);

	GetWorld()->GetTimerManager().ClearTimer(RangeAttackTaskMemory->PlayMontageHandle);
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

void UBTTask_RangeAttack::OnAttackNotifyBegin(FName NotifyName,
                                              const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName != TEXT("SpawnProjectile"))
	{
		return;
	}

	//Pretty much horrible, but it avoids me to do an AI Task just for this animation callback.
	ACharacter* SpawnInstigator{ Cast<ACharacter>(BranchingPointPayload.SkelMeshComponent->GetOwner()) };
	const AAIController* AIController{ Cast<AAIController>(SpawnInstigator->GetController()) };
	UBehaviorTreeComponent* BehaviorTreeComponent{ Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()) };
	const FBTRangeAttackTaskMemory* RangeAttackTaskMemory{
		CastNodeMemory<FBTRangeAttackTaskMemory>(AIController->GetBlackboardComponent())
	};

	if (!RangeAttackTaskMemory->bInitialized)
	{
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Aborted);
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
	
	if (!ProjectileActor)
	{
		return;
	}

	UProjectileMovementComponent* ProjectileMovementComponent{ ProjectileActor->GetProjectileMovementComponent() };

	ProjectileActor->SetDamage(RangeAttackTaskMemory->AttackDamage);
	ProjectileActor->FinishSpawning(ProjectileTransform);

	if (!RangeAttackTaskMemory->ThrowingBoneName.IsNone())
	{
		ProjectileActor->SetActorLocation(SpawnInstigator->GetMesh()->GetBoneLocation(RangeAttackTaskMemory->ThrowingBoneName));
	}
	
	const AActor* RetrievedActor{ RangeAttackTaskMemory->AttackedTarget.Get() };
	const FVector ProjectileVelocity{
		(RetrievedActor->GetActorLocation() - ProjectileActor->GetActorLocation()).GetSafeNormal() *
		ProjectileMovementComponent->InitialSpeed
	};
		
	ProjectileMovementComponent->Velocity = ProjectileVelocity;
}

void UBTTask_RangeAttack::LaunchMontageTimer(ACharacter* AICharacter, FBTRangeAttackTaskMemory* RangeAttackTaskMemory,
                                             UBehaviorTreeComponent* BehaviorTreeComponent, const float Rate)
{
	//Assign a lambda to have access to RangeAttackTaskMemory, BehaviorTreeComponent and AICharacter.
	GetWorld()->GetTimerManager().SetTimer(RangeAttackTaskMemory->PlayMontageHandle,
	   [this, AICharacter, RangeAttackTaskMemory, BehaviorTreeComponent]
	   {
	       OnPlayMontageTimerFinish(AICharacter, RangeAttackTaskMemory, BehaviorTreeComponent);
	   }, Rate, false);
}

void UBTTask_RangeAttack::OnPlayMontageTimerFinish(ACharacter* AICharacter,
	FBTRangeAttackTaskMemory* RangeAttackTaskMemory, UBehaviorTreeComponent* BehaviorTreeComponent)
{
	if (!AICharacter || !RangeAttackTaskMemory)
	{
		FinishLatentAbort(*BehaviorTreeComponent);
		return;
	}

	AICharacter->PlayAnimMontage(RangeAttackTaskMemory->AttackAnimationMontage.Get());

	BindEndAnimEvent(RangeAttackTaskMemory, BehaviorTreeComponent);
}

void UBTTask_RangeAttack::BindEndAnimEvent(FBTRangeAttackTaskMemory* RangeAttackTaskMemory,
                                           UBehaviorTreeComponent* BehaviorTreeComponent)
{
	//Assign a lambda to get access to the task memory and behavior tree component.
	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindLambda([this, RangeAttackTaskMemory, BehaviorTreeComponent]
	(UAnimMontage* Montage, bool bInterrupted)
	{
		OnEndMontage(RangeAttackTaskMemory, BehaviorTreeComponent, Montage, bInterrupted);
	});
	   	
	RangeAttackTaskMemory->AnimInstance->Montage_SetEndDelegate(OnMontageEnded,
																RangeAttackTaskMemory->AttackAnimationMontage.Get());
}

void UBTTask_RangeAttack::OnEndMontage(FBTRangeAttackTaskMemory* RangeAttackTaskMemory,
									   UBehaviorTreeComponent* OwnerComp, UAnimMontage* Montage, bool bInterrupted)
{
	if (!RangeAttackTaskMemory->bInitialized || !RangeAttackTaskMemory->SpawnedProjectile)
	{
		FinishLatentAbort(*OwnerComp);
		return;
	}

	ACharacter* AICharacter{ OwnerComp->GetAIOwner()->GetCharacter() };
	IDamageable* Damageable{ Cast<IDamageable>(RangeAttackTaskMemory->AttackedTarget.Get()) };

	if (!Damageable || !AICharacter)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	LaunchMontageTimer(AICharacter, RangeAttackTaskMemory, OwnerComp, RangeAttackTaskMemory->AttackCooldown);
}

EBlackboardNotificationResult UBTTask_RangeAttack::OnSpawnedProjectileKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{ CastNodeMemory<FBTRangeAttackTaskMemory>(&Blackboard) };
	AttackTaskMemory->SpawnedProjectile = Blackboard.GetValue<UBlackboardKeyType_Class>(ChangedKeyID);

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_RangeAttack::OnAttackCooldownKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{ CastNodeMemory<FBTRangeAttackTaskMemory>(&Blackboard) };
	AttackTaskMemory->AttackCooldown = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_RangeAttack::OnAttackDamageKeyValueChange(const UBlackboardComponent& Blackboard,
	FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{ CastNodeMemory<FBTRangeAttackTaskMemory>(&Blackboard) };
	AttackTaskMemory->AttackDamage = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_RangeAttack::OnAttackedTargetKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* AttackTaskMemory{ CastNodeMemory<FBTRangeAttackTaskMemory>(&Blackboard) };
	AttackTaskMemory->AttackedTarget = Cast<AActor>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));

	return EBlackboardNotificationResult::ContinueObserving;
}
