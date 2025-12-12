#include "IA/Characters/BaseAICharacter.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName("AiPawn");

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnDeath.AddDynamic(this, &ABaseAICharacter::OnDeath);
}

void ABaseAICharacter::OnDeath()
{
	Destroy();
}

bool ABaseAICharacter::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	HealthComponent->RemoveHealth(Damage);
	FString Msg = FString::Printf(
		TEXT("enemy hit, dealt %f damages"),
		Damage
	);
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Blue, Msg);

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

void ABaseAICharacter::SetMonsterData(UMonsterDataAsset* Data)
{
	UsedDataAsset = Data;
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
