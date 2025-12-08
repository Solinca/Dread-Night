#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwordCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API USwordCombatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	USwordCombatComponent();

	FTimerHandle AttackCooldownTimerHandle;

	bool bCanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackCooldown = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackRange = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackRadius = 80.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackDamage = 25.0f;

	void PerformAttackTrace();
	void ResetAttack();

public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();		
};
