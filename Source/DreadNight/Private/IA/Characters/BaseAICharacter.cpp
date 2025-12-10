#include "IA/Characters/BaseAICharacter.h"

#include "AIController.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
}

bool ABaseAICharacter::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	HealthComponent->RemoveHealth(Damage);

	return true;
}

void ABaseAICharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	AIController->SetGenericTeamId(TeamID);
}

FGenericTeamId ABaseAICharacter::GetGenericTeamId() const
{
	return AIController->GetGenericTeamId();
}

ETeamAttitude::Type ABaseAICharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	return AIController->GetTeamAttitudeTowards(Other);
}

void ABaseAICharacter::PossessedBy(AController* NewController)	
{
	Super::PossessedBy(NewController);

	AIController = Cast<AAIController>(NewController);

	UBlackboardComponent* BlackboardComponent{AIController->GetBlackboardComponent()};
	BP_OnDataAssetInitialization(BlackboardComponent, UsedDataAsset);
}

UMonsterDataAsset* ABaseAICharacter::GetMonsterData() const
{
	return UsedDataAsset;
}

void ABaseAICharacter::OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent, UMonsterDataAsset* MonsterDataAsset)
{
	GetMesh()->SetSkeletalMesh(MonsterDataAsset->GetMesh());

	HealthComponent->SetMaxHealth(MonsterDataAsset->GetMaxHealth());
}

void ABaseAICharacter::BP_OnDataAssetInitialization_Implementation(UBlackboardComponent* BlackboardComponent,
                                                                   UMonsterDataAsset* MonsterDataAsset)
{
	OnDataAssetInitialization(BlackboardComponent, MonsterDataAsset);
}
