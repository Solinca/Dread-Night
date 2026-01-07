#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Player/PlayerDataAsset.h"
#include "ConditionStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConditionStateChangedSignature, float, Value);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UConditionStateComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float CurrentHunger;

protected:
	UConditionStateComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UPlayerDataAsset> PlayerData;

public:
	UPROPERTY(BlueprintAssignable)
	FConditionStateChangedSignature OnHungerChanged;

	UFUNCTION()
	void AddHungerValue(float amount);

	UFUNCTION()
	void RemoveHungerValue(float amount);

	UFUNCTION()
	float GetHungerValueRatio();
};
