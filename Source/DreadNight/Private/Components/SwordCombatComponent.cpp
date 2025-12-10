#include "Components/SwordCombatComponent.h"
#include "Components/BoxComponent.h"
#include "DamageSystem/Interface/Damageable.h"

USwordCombatComponent::USwordCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USwordCombatComponent::Attack()
{
	if (IsAttacking)
	{
		return;
	}

	IsAttacking = true;

	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &USwordCombatComponent::ResetAttack, AttackCooldown, false);
}

bool USwordCombatComponent::GetIsAttacking()
{
	return IsAttacking;
}

void USwordCombatComponent::ResetAttack()
{
	IsAttacking = false;
}

void USwordCombatComponent::OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsAttacking && OtherActor != GetOwner() && OtherActor->Implements<UDamageable>())
	{
		if (IDamageable* dmg = Cast<IDamageable>(OtherActor))
		{
			dmg->TryApplyDamage(CurrentDamage, GetOwner());
		}
	}
}

void USwordCombatComponent::SetWeapon(UStaticMeshComponent* Mesh, float Damage, float Cooldown)
{
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &USwordCombatComponent::OnSwordOverlap);

	CurrentDamage = Damage;

	AttackCooldown = Cooldown;
}