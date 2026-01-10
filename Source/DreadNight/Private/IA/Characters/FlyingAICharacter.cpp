#include "IA/Characters/FlyingAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IA/DataAssets/FlyingMonsterDataAsset.h"
#include "Components/CapsuleComponent.h"

AFlyingAICharacter::AFlyingAICharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName("AiGroundPawn");

	UpFlyingCollision = CreateDefaultSubobject<UCapsuleComponent>("UpFlyingCollision");
	UpFlyingCollision->SetCollisionProfileName("AiFlyingPawn");
	UpFlyingCollision->SetupAttachment(RootComponent);
}

void AFlyingAICharacter::OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset)
{
	Super::OnDataAssetInitialization(BlackboardComponent, MonsterDataAsset);

	//The player character has a chance to not be set at the time SetupBlackboard is set.
	auto RetrievePlayer{[BlackboardComponent, this]
	{
		AActor* PlayerActor{GetWorld()->GetFirstPlayerController()->GetPawn()};
		BlackboardComponent->SetValueAsObject("TargetActor", PlayerActor);
	}};

	GetWorldTimerManager().SetTimerForNextTick(RetrievePlayer);

	if (UFlyingMonsterDataAsset* HostileMonsterDataAsset{Cast<UFlyingMonsterDataAsset>(MonsterDataAsset)})
	{
		BlackboardComponent->SetValueAsFloat("AttackRange", HostileMonsterDataAsset->GetAttackRange());
		BlackboardComponent->SetValueAsFloat("AcceptableRadius", HostileMonsterDataAsset->GetAcceptableRadius());
		BlackboardComponent->SetValueAsFloat("AttackCooldown", HostileMonsterDataAsset->GetAttackCooldown());
		BlackboardComponent->SetValueAsFloat("FleeRange", HostileMonsterDataAsset->GetFleeRange());
		BlackboardComponent->SetValueAsObject("SpawnableAI", HostileMonsterDataAsset->GetSpawnableAI());
		BlackboardComponent->SetValueAsObject("AttackAnimationMontage", HostileMonsterDataAsset->GetAttackAnimationMontage());
	}
}
