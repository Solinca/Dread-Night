#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/ProjectileActor.h"
#include "BowCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UBowCombatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	UBowCombatComponent();

	FTimerHandle ShotCooldownTimer;

	bool bCanShoot;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsAiming;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ShotCooldown = 0.8f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectileActor> ArrowProjectileClass;
	UPROPERTY()
	TWeakObjectPtr<AProjectileActor> CurrentArrow;

	void SpawnArrow();
	void ResetShot();

public:
	//à appeler dans le player controller
	UFUNCTION()
	void SetAiming(bool bAiming);
	UFUNCTION()
	void Shoot();			
};
