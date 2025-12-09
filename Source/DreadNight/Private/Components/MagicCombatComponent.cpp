#include "Components/MagicCombatComponent.h"

UMagicCombatComponent::UMagicCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMagicCombatComponent::CastFireball()
{
	AActor* Fireball = GetWorld()->SpawnActor<AActor>(FireballClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
}