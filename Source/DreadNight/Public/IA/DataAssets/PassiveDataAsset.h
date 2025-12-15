#pragma once

#include "CoreMinimal.h"
#include "MonsterDataAsset.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "PassiveDataAsset.generated.h"

UCLASS()
class DREADNIGHT_API UPassiveDataAsset : public UMonsterDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fleeing")
	float FleeTime{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fleeing")
	float FleeingSpeed{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fleeing")
	float FleeingAcceptableRadius{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fleeing")
	float FleeingWaitTime{0.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fleeing")
	TObjectPtr<UEnvQuery> FleeingEQS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wandering")
	float WanderingSpeed{0.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wandering")
	float WanderAcceptableRadius{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wandering")
	float WanderWaitTime{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wandering")
	TObjectPtr<UEnvQuery> WanderEQS;
	
public:
	[[nodiscard]] float GetFleeTime() const;
	[[nodiscard]] float GetFleeingSpeed() const;
	[[nodiscard]] float GetFleeingAcceptableRadius() const;
	[[nodiscard]] float GetFleeingWaitTime() const;
	[[nodiscard]] UEnvQuery* GetFleeingEQS() const;

	[[nodiscard]] float GetWanderingSpeed() const;
	[[nodiscard]] float GetWanderAcceptableRadius() const;
	[[nodiscard]] float GetWanderWaitTime() const;
	[[nodiscard]] UEnvQuery* GetWanderEQS() const;
};
