#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Player/PlayerDataAsset.h"
#include "SaveSystem/SavableObject.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStaminaChanged, float, Stamina, float, MaxStamina);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREADNIGHT_API UStaminaComponent : public UActorComponent, public ISavableObject
{
	GENERATED_BODY()

private:
	float CurrentStamina;

	bool CanRegen = false;

protected:	
	UStaminaComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UPlayerDataAsset> PlayerData;

public:
	FTimerHandle CoolDownTimer;

	UPROPERTY(BlueprintAssignable)
	FStaminaChanged OnStaminaChanged;

	UFUNCTION()
	void AddStamina(float amount);

	UFUNCTION()
	void RemoveStamina(float amount);

	UFUNCTION()
	float GetStaminaRatio();

	UFUNCTION()
	void RegenStamina(float DeltaTime);

	UFUNCTION()
	void SetCanRegen(bool value);

	UFUNCTION()
	float GetCurrentStamina();
};
