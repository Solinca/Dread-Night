#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREADNIGHT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values", meta = (ClampMin = 1.f))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float CurrentHealth = 100.f;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;

	void SetMaxHealth(float amount);

	void AddMaxHealth(float amount, bool doesModifyCurrentHealth = false);

	void AddHealth(float amount);

	void RemoveMaxHealth(float amount, bool doesModifyCurrentHealth = false);

	void RemoveHealth(float amount);

	UFUNCTION(BlueprintCallable)
	float GetHealthRatio();
};
