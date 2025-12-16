#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FManaChanged, float, Mana, float, MaxMana);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UManaComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values", meta = (ClampMin = 1.f))
	float MaxMana = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float CurrentMana = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float RegenSpeed = 10.f;//en %/s

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float RegenCooldown = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	bool CanRegen = false;

public:

	FTimerHandle CoolDownTimer;

	UPROPERTY(BlueprintAssignable)
	FManaChanged OnManaChanged;

	UFUNCTION()
	void AddMaxMana(float amount);

	UFUNCTION()
	void AddMana(float amount);

	UFUNCTION()
	void RemoveMaxMana(float amount);

	UFUNCTION()
	void RemoveMana(float amount);

	UFUNCTION()
	float GetManaRatio();

	UFUNCTION()
	void RegenMana(float DeltaTime);

	UFUNCTION()
	void SetCanRegen(bool value);

	UFUNCTION()
	float GetRegenCoolDown();
		
};
