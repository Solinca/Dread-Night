#include "IA/Characters/RangeAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IA/DataAssets/RangeMonsterDataAsset.h"

void ARangeAICharacter::OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent,
                                                  UMonsterDataAsset* MonsterDataAsset)
{
	Super::OnDataAssetInitialization(BlackboardComponent, MonsterDataAsset);

	//The player character has a chance to not be set at the time SetupBlackboard is set.
	auto RetrievePlayer{[BlackboardComponent, this]
	{
		AActor* PlayerActor{GetWorld()->GetFirstPlayerController()->GetPawn()};
		BlackboardComponent->SetValueAsObject("TargetActor", PlayerActor);
	}};

	GetWorldTimerManager().SetTimerForNextTick(RetrievePlayer);

	if (URangeMonsterDataAsset* RangeMonsterDataAsset{Cast<URangeMonsterDataAsset>(MonsterDataAsset)})
	{
		BlackboardComponent->SetValueAsFloat("AttackRange", RangeMonsterDataAsset->GetAttackRange());
		BlackboardComponent->SetValueAsFloat("AcceptableRadius", RangeMonsterDataAsset->GetAcceptableRadius());
		BlackboardComponent->SetValueAsFloat("AttackCooldown", RangeMonsterDataAsset->GetAttackCooldown());
		BlackboardComponent->SetValueAsFloat("AttackDamage", RangeMonsterDataAsset->GetAttackDamage());
		BlackboardComponent->SetValueAsFloat("FleeRange", RangeMonsterDataAsset->GetFleeRange());
		BlackboardComponent->SetValueAsClass("SpawnedProjectile", RangeMonsterDataAsset->GetSpawnedProjectile());
		BlackboardComponent->SetValueAsObject("AttackAnimationMontage", RangeMonsterDataAsset->GetAttackAnimationMontage());
		BlackboardComponent->SetValueAsName("ThrowingBoneName", RangeMonsterDataAsset->GetThrowBoneName());
	}
}
