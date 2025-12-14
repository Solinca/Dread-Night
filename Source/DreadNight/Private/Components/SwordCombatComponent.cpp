#include "Components/SwordCombatComponent.h"
#include "Components/BoxComponent.h"
#include "DamageSystem/Interface/Damageable.h"

USwordCombatComponent::USwordCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

void USwordCombatComponent::ResetAttack()
{
	IsAttacking = false;
}

void USwordCombatComponent::Attack()
{
	IsAttacking = true;
}

void USwordCombatComponent::SetWeapon(UStaticMeshComponent* Mesh, float Damage)
{
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &USwordCombatComponent::OnSwordOverlap);

	CurrentDamage = Damage;
}