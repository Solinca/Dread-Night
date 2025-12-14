#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwordCombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API USwordCombatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float CurrentDamage;

	bool IsAttacking = false;

protected:	
	USwordCombatComponent();

	UFUNCTION()
	void OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking() { return IsAttacking; }

public:
	void Attack();

	void SetWeapon(UStaticMeshComponent* Mesh, float Damage);
};
