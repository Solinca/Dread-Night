#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwordCombatComponent.generated.h"

class UWeaponDataAsset;
class UItemInstance_Weapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API USwordCombatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool IsAttacking = false;
	
	UPROPERTY(Transient)
	UWeaponDataAsset* CurrentWeapon = nullptr;

protected:
	USwordCombatComponent();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

public:
	UFUNCTION()
	void OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking() { return IsAttacking; }

	UFUNCTION(BlueprintCallable)
	UWeaponDataAsset* GetCurrentWeapon() { return CurrentWeapon; }
	
	void Attack();
	
	void SetWeapon(UWeaponDataAsset* Weapon);
};
