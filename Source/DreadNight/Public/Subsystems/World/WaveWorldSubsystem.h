#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WaveWorldSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveEndSignature);

UCLASS()
class DREADNIGHT_API UWaveWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnNightStart();

	void EndNight();

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public:
	FOnWaveEndSignature OnWaveEnd;
};
