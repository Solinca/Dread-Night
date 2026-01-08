#include "SaveSystem/RespawnComponent.h"
#include "Subsystems/World/DayCycleSubSystem.h"

URespawnComponent::URespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URespawnComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UDayCycleSubSystem>()->OnDawnStart.AddDynamic(this, &ThisClass::URespawnComponent::OnNewDayUpdate);
}


void URespawnComponent::OnNewDayUpdate()
{
	NightBeforeRespawn--;

	if (bIsDead && NightBeforeRespawn <= 0)
	{
		Respawn();
	}
}

void URespawnComponent::Despawn()
{
	NightBeforeRespawn = RespawnNightNumber;
	
	bIsDead = true;
	
	AActor* Owner = GetOwner();
	
	Owner->SetActorEnableCollision(false);
	
	Owner->SetActorHiddenInGame(true);

	TArray<AActor*> AttachedActors;

	Owner->GetAttachedActors(AttachedActors);

	for (AActor* actor : AttachedActors)
	{
		actor->SetActorHiddenInGame(true);
	}
}

void URespawnComponent::Respawn()
{
	bIsDead = false;
	
	AActor* Owner = GetOwner();
	
	Owner->SetActorEnableCollision(true);
	
	Owner->SetActorHiddenInGame(false);
	
	OnRespawn.ExecuteIfBound();
}

void URespawnComponent::OnPostLoad()
{
	if (!bIsDead)
	{
		Respawn();
	}
	else
	{
		Despawn();
	}
	
	++NightBeforeRespawn;
}
