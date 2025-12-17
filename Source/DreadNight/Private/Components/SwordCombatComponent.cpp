#include "Components/SwordCombatComponent.h"
#include "Components/BoxComponent.h"
#include "DamageSystem/Interface/Damageable.h"
#include "Items/Data/WeaponDataAsset.h"
#include "Items/Data/WeaponDataAsset.h"

USwordCombatComponent::USwordCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USwordCombatComponent::OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsAttacking && OtherActor != GetOwner() && OtherActor->Implements<UDamageable>())
	{
		if (IDamageable* Damageable = Cast<IDamageable>(OtherActor))
		{
			Damageable->TryApplyDamage(CurrentWeapon->Damage, GetOwner());
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

void USwordCombatComponent::SetComponentMesh(UStaticMeshComponent* Mesh)
{
	if (CurrentStaticMesh)
	{
		CurrentStaticMesh->OnComponentBeginOverlap.RemoveDynamic(this, &USwordCombatComponent::OnSwordOverlap);
	}	
	CurrentStaticMesh = Mesh;
	CurrentStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &USwordCombatComponent::OnSwordOverlap);

}

void USwordCombatComponent::SetWeapon(UWeaponDataAsset* Weapon)
{
	CurrentWeapon = Weapon;
	if (CurrentStaticMesh)
	{
		CurrentStaticMesh->SetStaticMesh(Weapon->WeaponMesh);
	}
	
}
 