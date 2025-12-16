#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConditionStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FConditionStateChanged, float, Value, bool, IsState);


UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EConditionState : uint8
{
	NONE = 0,
	HUNGRY = 1 << 0,  // 1
	AFRAID = 1 << 1   // 4
};
ENUM_CLASS_FLAGS(EConditionState)



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UConditionStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UConditionStateComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/// <summary>
	/// Combiner plusieurs �tats :
	/// EConditionState State = EConditionState::HUNGRY | EConditionState::THIRSTY;
	/// 
	/// V�rifier si un �tat est pr�sent :
	/// bool bIsHungry = EnumHasAnyFlags(State, EConditionState::HUNGRY);
	/// 
	/// Ajouter un �tat :
	/// State |= EConditionState::AFRAID;
	/// 
	/// Retirer un �tat :
	/// State &= ~EConditionState::THIRSTY;
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	EConditionState States = EConditionState::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	float HungerValue = 100.f;// 0 - 100 where 100 is NOT hungry

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State", meta = (ClampMin = 1.f))
	float HungerMaxValue = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	float DecreaseHungerRate = 0.1f;// in unit/s

public:


	UPROPERTY(BlueprintAssignable)
	FConditionStateChanged OnHungerChanged;


	UFUNCTION()
	void AddHungerValue(float amount);

	UFUNCTION()
	void RemoveHungerValue(float amount);

 	UFUNCTION()
	void ClearStates();

	UFUNCTION()
	float GetHungerValueRatio();

};
