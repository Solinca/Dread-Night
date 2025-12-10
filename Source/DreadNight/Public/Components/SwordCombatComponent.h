#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwordCombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API USwordCombatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	FTimerHandle AttackCooldownTimerHandle;

	float CurrentDamage;

	float AttackCooldown;

	bool IsAttacking = false;

	void ResetAttack();

protected:	
	USwordCombatComponent();

	UFUNCTION()
	void OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking();

public:
	void Attack();

	void SetWeapon(UStaticMeshComponent* Mesh, float Damage, float Cooldown);
};
