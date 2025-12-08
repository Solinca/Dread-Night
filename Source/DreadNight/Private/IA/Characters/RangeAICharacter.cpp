#include "IA/Characters/RangeAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IA/DataAssets/BlackboardMonsterDataAsset.h"

void ARangeAICharacter::OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent,
                                                  UMonsterDataAsset* MonsterDataAsset)
{
	//The player character has a chance to not be set at the time SetupBlackboard is set.
	auto RetrievePlayer{[BlackboardComponent, this]
	{
		AActor* PlayerActor{GetWorld()->GetFirstPlayerController()->GetPawn()};
		BlackboardComponent->SetValueAsObject("TargetActor", PlayerActor);
	}};

	GetWorldTimerManager().SetTimerForNextTick(RetrievePlayer);

	UBlackboardMonsterDataAsset* BBMonsterDataAsset{Cast<UBlackboardMonsterDataAsset>(MonsterDataAsset)};

	BlackboardComponent->SetValueAsFloat("AttackRange", BBMonsterDataAsset->GetAttackRange());
	BlackboardComponent->SetValueAsFloat("AcceptableRadius", BBMonsterDataAsset->GetAcceptableRadius());

	GetMesh()->SetSkeletalMesh(MonsterDataAsset->GetMesh());
}
