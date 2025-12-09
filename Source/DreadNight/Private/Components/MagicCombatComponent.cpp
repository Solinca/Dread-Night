#include "Components/MagicCombatComponent.h"

UMagicCombatComponent::UMagicCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMagicCombatComponent::CastFireball()
{
	// Implementation for casting a fireball
	AActor* Fireball = GetWorld()->SpawnActor<AActor>(FireballClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
}