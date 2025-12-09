#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageSystem/Interface/Damageable.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChanged, float, CurrentHealth, float, MaxHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREADNIGHT_API UHealthComponent : public UActorComponent, public IDamageable
{
	GENERATED_BODY()

protected:
	UHealthComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values", meta = (ClampMin = 1.f))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float CurrentHealth;
	
public:


	UPROPERTY(BlueprintAssignable)
	FHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FHealthChanged OnDamageTaken;

	UFUNCTION()
	void TakeDamage(float Damage, AActor* DamageInstigator);

	UFUNCTION()
	void AddMaxHealth(float amount);

	UFUNCTION()
	void AddHealth(float amount);

	UFUNCTION()
	void RemoveMaxHealth(float amount);

	UFUNCTION()
	void Death();

	UFUNCTION()
	float GetHealthRatio();
};
