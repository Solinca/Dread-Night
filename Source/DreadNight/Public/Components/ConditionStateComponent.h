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
	THIRSTY = 1 << 1, // 2
	AFRAID = 1 << 2   // 4
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
	/// Combiner plusieurs états :
	/// EConditionState State = EConditionState::HUNGRY | EConditionState::THIRSTY;
	/// 
	/// Vérifier si un état est présent :
	/// bool bIsHungry = EnumHasAnyFlags(State, EConditionState::HUNGRY);
	/// 
	/// Ajouter un état :
	/// State |= EConditionState::AFRAID;
	/// 
	/// Retirer un état :
	/// State &= ~EConditionState::THIRSTY;
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	EConditionState States = EConditionState::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	float HungerValue = 100.f;// 0 - 100 where 100 is NOT hungry

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State", meta = (ClampMin = 1.f))
	float HungerMaxValue = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	float ThirstValue = 100.f;// 0 - 100 where 100 is NOT thirsty

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State", meta = (ClampMin = 1.f))
	float ThirstMaxValue = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	float SanityValue = 100.f;// 0 - 100 where 100 is NOT crazy

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State", meta = (ClampMin = 1.f))
	float SanityMaxValue = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	float DecreaseHungerRate = 0.1f;// in unit/s

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
	float DecreaseThirstRate = 0.1f;// in unit/s

public:


	UPROPERTY(BlueprintAssignable)
	FConditionStateChanged OnHungerChanged;

	UPROPERTY(BlueprintAssignable)
	FConditionStateChanged OnThirstChanged;

	UPROPERTY(BlueprintAssignable)
	FConditionStateChanged OnSanityChanged;


	UFUNCTION()
	void AddHungerValue(float amount);

	UFUNCTION()
	void RemoveHungerValue(float amount);

	UFUNCTION()
	void AddThirstValue(float amount);

	UFUNCTION()
	void RemoveThirstValue(float amount);

	UFUNCTION()
	void AddSanityValue(float amount);

	UFUNCTION()
	void RemoveSanityValue(float amount);

	UFUNCTION()
	void ClearStates();

	UFUNCTION()
	float GetHungerValueRatio();

	UFUNCTION()
	float GetThirstValueRatio();

	UFUNCTION()
	float GetSanityValueRatio();
		
};
