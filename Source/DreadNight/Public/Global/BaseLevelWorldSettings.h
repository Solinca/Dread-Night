#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "BaseLevelWorldSettings.generated.h"

UCLASS()
class DREADNIGHT_API ABaseLevelWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float Test = 2.f;
};
