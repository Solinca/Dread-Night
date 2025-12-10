#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStaminaChanged, float, Stamina, float, MaxStamina);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREADNIGHT_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	UStaminaComponent();


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values", meta = (ClampMin = 1.f))
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float RegenSpeed;//en %/s

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float RegenCooldown = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	bool CanRegen = false;


public:

	FTimerHandle CoolDownTimer;

	UPROPERTY(BlueprintAssignable)
	FStaminaChanged OnStaminaChanged;

	UFUNCTION()
	void AddMaxStamina(float amount);

	UFUNCTION()
	void AddStamina(float amount);

	UFUNCTION()
	void RemoveMaxStamina(float amount);

	UFUNCTION()
	void RemoveStamina(float amount);

	UFUNCTION()
	float GetStaminaRatio();

	UFUNCTION()
	void RegenStamina(float DeltaTime);

	UFUNCTION()
	void SetCanRegen(bool value);

	UFUNCTION()
	float GetRegenCoolDown();
};
