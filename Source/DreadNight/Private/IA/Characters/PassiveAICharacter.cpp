#include "IA/Characters/PassiveAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IA/DataAssets/PassiveDataAsset.h"
#include "SaveSystem/RespawnComponent.h"

APassiveAICharacter::APassiveAICharacter()
{
	RespawnComponent = CreateDefaultSubobject<URespawnComponent>("Respawn Component");
}

void APassiveAICharacter::BeginPlay()
{
	Super::BeginPlay();

	RespawnComponent->OnRespawn.BindLambda([this]
	{
		HealthComponent->SetMaxHealth(HealthComponent->GetMaxHealth());

		GetWorld()->GetTimerManager().SetTimer(IdleSoundIntervalTimer, this, &ABaseAICharacter::PlayIdleSound, FMath::FRandRange(UsedDataAsset->MinIntervalBetweenIdleSound, UsedDataAsset->MaxIntervalBetweenIdleSound), false);
	});
}

void APassiveAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void APassiveAICharacter::OnDataAssetInitialization(UBlackboardComponent* BlackboardComponent,
                                                    UMonsterDataAsset* MonsterDataAsset)
{
	Super::OnDataAssetInitialization(BlackboardComponent, MonsterDataAsset);

	if (UPassiveDataAsset* PassiveDataAsset{ Cast<UPassiveDataAsset>(MonsterDataAsset) })
	{
		BlackboardComponent->SetValueAsFloat("FleeingAcceptableRadius", PassiveDataAsset->GetFleeingAcceptableRadius());
		BlackboardComponent->SetValueAsFloat("FleeingWaitTime", PassiveDataAsset->GetFleeingWaitTime());
		BlackboardComponent->SetValueAsObject("FleeingEQS", PassiveDataAsset->GetFleeingEQS());

		BlackboardComponent->SetValueAsFloat("WanderAcceptableRadius", PassiveDataAsset->GetWanderAcceptableRadius());
		BlackboardComponent->SetValueAsFloat("WanderWaitTime", PassiveDataAsset->GetWanderWaitTime());
		BlackboardComponent->SetValueAsObject("WanderEQS", PassiveDataAsset->GetWanderEQS());
		
		GetCharacterMovement()->MaxWalkSpeed = PassiveDataAsset->GetWanderingSpeed();	
	}
}

bool APassiveAICharacter::TryApplyDamage(float Damage, AActor* DamageInstigator)
{
	if (UPassiveDataAsset* PassiveDataAsset{ Cast<UPassiveDataAsset>(UsedDataAsset) })
	{
		GetWorldTimerManager().SetTimer(FleeTimerHandle, this, &APassiveAICharacter::OnFleeTimerFinish,
		                                PassiveDataAsset->GetFleeTime(), false);

		UBlackboardComponent* BlackboardComponent{AIController->GetBlackboardComponent()};
		BlackboardComponent->SetValueAsBool("IsFleeing", true);

		GetCharacterMovement()->MaxWalkSpeed = PassiveDataAsset->GetFleeingSpeed();
	}

	return Super::TryApplyDamage(Damage, DamageInstigator);
}

void APassiveAICharacter::OnDeath()
{
	DropLoot();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), UsedDataAsset->MonsterDeath, GetActorLocation());

	GetWorld()->GetTimerManager().ClearTimer(IdleSoundIntervalTimer);

	RespawnComponent->Despawn();
}
 

void APassiveAICharacter::OnFleeTimerFinish()
{
	UBlackboardComponent* BlackboardComponent{AIController->GetBlackboardComponent()};
	BlackboardComponent->SetValueAsBool("IsFleeing", false);

	if (UPassiveDataAsset* PassiveDataAsset{ Cast<UPassiveDataAsset>(UsedDataAsset) })
	{
		GetCharacterMovement()->MaxWalkSpeed = PassiveDataAsset->GetWanderingSpeed();	
	}
}
