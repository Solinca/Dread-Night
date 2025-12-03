#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BowCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UBowCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBowCombatComponent();

protected:
	virtual void BeginPlay() override;

	FTimerHandle ShotCooldownTimer;

	bool bCanShoot;
	bool bIsAiming;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ShotCooldown = 0.8f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> ArrowProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ArrowSpeed = 3000.f;

public:

	//Appelé par le Character quand clic droit enfoncé ou relâché
	UFUNCTION(BlueprintCallable)
	void SetAiming(bool bAiming);

	// Appelé par le Character quand clic gauche
	UFUNCTION(BlueprintCallable)
	void Shoot();

protected:
	void SpawnArrowProjectile();
	void ResetShot();		
};
