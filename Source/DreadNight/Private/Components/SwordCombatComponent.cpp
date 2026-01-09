#include "Components/SwordCombatComponent.h"
#include "Components/BoxComponent.h"
#include "DamageSystem/Interface/Damageable.h"
#include "Items/Data/WeaponDataAsset.h"
#include "Player/PlayerCharacter.h"

USwordCombatComponent::USwordCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USwordCombatComponent::OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsAttacking && CurrentWeapon && OtherActor != GetOwner() && OtherActor->Implements<UDamageable>())
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

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	UStaminaComponent* StaminaComponent = Player->GetStaminaComponent();

	// START REGEN STAMINA
	Player->GetWorldTimerManager().SetTimer(StaminaComponent->CoolDownTimer,
		[=] {StaminaComponent->SetCanRegen(true); },
		Player->GetData()->TimeBeforeStartRegenStamina, false
	);
}

void USwordCombatComponent::Attack()
{
	IsAttacking = true;
}

void USwordCombatComponent::SetWeapon(UWeaponDataAsset* Weapon)
{
	CurrentWeapon = Weapon;
}
 