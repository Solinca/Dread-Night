#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwordCombatComponent.generated.h"

class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API USwordCombatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	USwordCombatComponent();
	virtual void BeginPlay() override;

	FTimerHandle AttackCooldownTimerHandle;
	FTimerHandle CollisionCooldownTimerHandle;

	bool bCanAttack;
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackCooldown = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackDamage = 25.0f;
	UPROPERTY()
	TWeakObjectPtr<UBoxComponent> SwordHitBox;
	UPROPERTY()
	TArray<AActor*> HitActors;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float CollisionEnableTime = 0.3f;

	void EnableSwordCollision();
	void DisableSwordCollision();
	void ResetAttack();

	UFUNCTION()
	void OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();
	void SetHitBoxComponent(UBoxComponent* HitBox);
	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking();
};
