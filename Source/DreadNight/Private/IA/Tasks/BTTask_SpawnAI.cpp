#include "IA/Tasks/BTTask_SpawnAI.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "DamageSystem/Interface/Damageable.h"
#include "GameFramework/Character.h"
#include "Subsystems/World/WaveWorldSubsystem.h"
#include <IA/Characters/PassiveAICharacter.h>

UBTTask_SpawnAI::UBTTask_SpawnAI()
{
	NodeName = "Spawn AI";

	bNotifyTaskFinished = true;

	AttackedTarget.AllowNoneAsValue(false);
	AttackedTarget.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SpawnAI, AttackedTarget),
	                               AActor::StaticClass());
	//For now the base class will be equal to this Projectile Actor class, and later I will see if it needs to be changed.
	SpawnableAI.SetBaseClass(USpawnableAIContainer::StaticClass());
}

EBTNodeResult::Type UBTTask_SpawnAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* SpawnAITaskMemory{ CastNodeMemory<FBTSpawnAITaskMemory>(NodeMemory) };
	if (SpawnAITaskMemory->bInitialized)
	{
		return EBTNodeResult::Succeeded;
	}

	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };

	SpawnAITaskMemory->AttackCooldown = AttackCooldown.GetValue(OwnerComp);
	
	SpawnAITaskMemory->AttackedTarget = Cast<AActor>(
		BlackboardComponent->GetValue<UBlackboardKeyType_Object>(AttackedTarget.GetSelectedKeyID()));

	SpawnAITaskMemory->SpawnableAI = Cast<USpawnableAIContainer>(SpawnableAI.GetValue(OwnerComp));
	
	SpawnAITaskMemory->AttackAnimationMontage = AttackAnimationMontage.GetValue<UAnimMontage>(OwnerComp);
	
	SpawnAITaskMemory->AnimInstance = OwnerComp.GetAIOwner()->GetCharacter()->GetMesh()->GetAnimInstance();

	SpawnAITaskMemory->AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UBTTask_SpawnAI::OnAttackNotifyBegin);

	SpawnAITaskMemory->MonsterAttackSound = MonsterAttackSound.GetValue<USoundBase>(OwnerComp);

	if (!SpawnAITaskMemory->AttackedTarget.IsValid() ||
		!SpawnAITaskMemory->AttackedTarget->Implements<UDamageable>() ||
		!SpawnAITaskMemory->SpawnableAI.IsValid())
	{
		return EBTNodeResult::Failed;
	}

	RegisterToKeyIdChecked(SpawnableAI, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_SpawnAI::OnSpawnedProjectileKeyValueChange);
	RegisterToKeyIdChecked(AttackCooldown, OwnerComp, BlackboardComponent, this,
	                       &UBTTask_SpawnAI::OnAttackCooldownKeyValueChange);

	BlackboardComponent->RegisterObserver(AttackedTarget.GetSelectedKeyID(), this,
	                                      FOnBlackboardChangeNotification::CreateUObject(
		                                      this, &UBTTask_SpawnAI::OnAttackedTargetKeyValueChange));

	AAIController* AIController{ OwnerComp.GetAIOwner() };
	AIController->SetFocus(SpawnAITaskMemory->AttackedTarget.Get());

	SpawnAITaskMemory->bInitialized = true;

	constexpr float StartingRate{ 0.1f };
	LaunchMontageTimer(AIController->GetCharacter(), SpawnAITaskMemory, &OwnerComp, StartingRate);

	return EBTNodeResult::InProgress;
}

void UBTTask_SpawnAI::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                         EBTNodeResult::Type TaskResult)
{
	auto* SpawnAITaskMemory{ CastNodeMemory<FBTSpawnAITaskMemory>(NodeMemory) };
	SpawnAITaskMemory->bInitialized = false;
	SpawnAITaskMemory->AttackedTarget.Reset();

	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	BlackboardComponent->UnregisterObserversFrom(this);

	AAIController* AIController{ OwnerComp.GetAIOwner() };
	AIController->ClearFocus(EAIFocusPriority::LastFocusPriority);

	SpawnAITaskMemory->AnimInstance->OnPlayMontageNotifyBegin.RemoveAll(this);

	//Need to check this because MontageEnded can be equal to nullptr.
	if (FOnMontageEnded* MontageEnded = SpawnAITaskMemory->AnimInstance->Montage_GetEndedDelegate(
		SpawnAITaskMemory->AttackAnimationMontage.Get()))
	{
		MontageEnded->Unbind();
	}

	//Then do a Blend out time to avoid cutting the animation and instead does a smooth transition.
	SpawnAITaskMemory->AnimInstance->Montage_Stop(0.25f);

	GetWorld()->GetTimerManager().ClearTimer(SpawnAITaskMemory->PlayMontageHandle);
}

void UBTTask_SpawnAI::InitializeFromAsset(UBehaviorTree& Asset)
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

uint16 UBTTask_SpawnAI::GetInstanceMemorySize() const
{
	return sizeof(FBTSpawnAITaskMemory);
}

void UBTTask_SpawnAI::OnAttackNotifyBegin(FName NotifyName,
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
	const FBTSpawnAITaskMemory* SpawnAITaskMemory{
		CastNodeMemory<FBTSpawnAITaskMemory>(AIController->GetBlackboardComponent())
	};

	if (!SpawnAITaskMemory->bInitialized)
	{
		FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Aborted);
		return;
	}

	FTransform AITransform;
	AITransform.SetScale3D(FVector::One());
	AITransform.SetLocation(SpawnInstigator->GetActorLocation());
	AITransform.SetRotation(SpawnInstigator->GetActorRotation().Quaternion());

	TArray<FSpawnableData>& SpawnableAIDatas{ SpawnAITaskMemory->SpawnableAI->SpawnableAI };
	FSpawnableData& SpawnableAIData { SpawnableAIDatas[FMath::RandRange(0, SpawnableAIDatas.Num() - 1)] };
	
	ABaseAICharacter* BaseAICharacter{
		GetWorld()->SpawnActorDeferred<ABaseAICharacter>(
			SpawnableAIData.AIClass,
			AITransform,
			nullptr,
			SpawnInstigator,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		)
	};

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnAITaskMemory->MonsterAttackSound.Get(), SpawnInstigator->GetActorLocation());
	
	if (!BaseAICharacter)
	{
		return;
	}

	if (SpawnableAIData.AIClass->GetSuperClass() != APassiveAICharacter::StaticClass())
	{
		BaseAICharacter->SetMonsterData(SpawnableAIData.AIData);

		UWaveWorldSubsystem::Get(GetWorld())->RegisterAI(BaseAICharacter);
	}
	
	BaseAICharacter->FinishSpawning(AITransform);
}

void UBTTask_SpawnAI::LaunchMontageTimer(ACharacter* AICharacter, FBTSpawnAITaskMemory* SpawnAITaskMemory,
                                             UBehaviorTreeComponent* BehaviorTreeComponent, const float Rate)
{
	//Assign a lambda to have access to SpawnAITaskMemory, BehaviorTreeComponent and AICharacter.
	GetWorld()->GetTimerManager().SetTimer(SpawnAITaskMemory->PlayMontageHandle,
	   [this, AICharacter, SpawnAITaskMemory, BehaviorTreeComponent]
	   {
	       OnPlayMontageTimerFinish(AICharacter, SpawnAITaskMemory, BehaviorTreeComponent);
	   }, Rate, false);
}

void UBTTask_SpawnAI::OnPlayMontageTimerFinish(ACharacter* AICharacter,
	FBTSpawnAITaskMemory* SpawnAITaskMemory, UBehaviorTreeComponent* BehaviorTreeComponent)
{
	if (!AICharacter || !SpawnAITaskMemory)
	{
		FinishLatentAbort(*BehaviorTreeComponent);
		return;
	}

	AICharacter->PlayAnimMontage(SpawnAITaskMemory->AttackAnimationMontage.Get());

	BindEndAnimEvent(SpawnAITaskMemory, BehaviorTreeComponent);
}

void UBTTask_SpawnAI::BindEndAnimEvent(FBTSpawnAITaskMemory* SpawnAITaskMemory,
                                           UBehaviorTreeComponent* BehaviorTreeComponent)
{
	//Assign a lambda to get access to the task memory and behavior tree component.
	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindLambda([this, SpawnAITaskMemory, BehaviorTreeComponent]
	(UAnimMontage* Montage, bool bInterrupted)
	{
		OnEndMontage(SpawnAITaskMemory, BehaviorTreeComponent, Montage, bInterrupted);
	});
	   	
	SpawnAITaskMemory->AnimInstance->Montage_SetEndDelegate(OnMontageEnded,
																SpawnAITaskMemory->AttackAnimationMontage.Get());
}

void UBTTask_SpawnAI::OnEndMontage(FBTSpawnAITaskMemory* SpawnAITaskMemory,
									   UBehaviorTreeComponent* OwnerComp, UAnimMontage* Montage, bool bInterrupted)
{
	if (!SpawnAITaskMemory->bInitialized || !SpawnAITaskMemory->SpawnableAI.IsValid())
	{
		FinishLatentAbort(*OwnerComp);
		return;
	}

	ACharacter* AICharacter{ OwnerComp->GetAIOwner()->GetCharacter() };

	if (!SpawnAITaskMemory->AttackedTarget.IsValid() || !AICharacter)
	{
		FinishLatentAbort(*OwnerComp);
		return;
	}

	LaunchMontageTimer(AICharacter, SpawnAITaskMemory, OwnerComp, SpawnAITaskMemory->AttackCooldown);
}

EBlackboardNotificationResult UBTTask_SpawnAI::OnSpawnedProjectileKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* SpawnAITaskMemory{ CastNodeMemory<FBTSpawnAITaskMemory>(&Blackboard) };
	SpawnAITaskMemory->SpawnableAI = Cast<USpawnableAIContainer>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_SpawnAI::OnAttackCooldownKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* SpawnAITaskMemory{ CastNodeMemory<FBTSpawnAITaskMemory>(&Blackboard) };
	SpawnAITaskMemory->AttackCooldown = Blackboard.GetValue<UBlackboardKeyType_Float>(ChangedKeyID);

	return EBlackboardNotificationResult::ContinueObserving;
}

EBlackboardNotificationResult UBTTask_SpawnAI::OnAttackedTargetKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	auto* SpawnAITaskMemory{ CastNodeMemory<FBTSpawnAITaskMemory>(&Blackboard) };
	SpawnAITaskMemory->AttackedTarget = Cast<AActor>(Blackboard.GetValue<UBlackboardKeyType_Object>(ChangedKeyID));

	return EBlackboardNotificationResult::ContinueObserving;
}
