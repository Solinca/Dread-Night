#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNightStartSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveEndSignature);

UCLASS()
class DREADNIGHT_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	FOnNightStartSignature OnNightStart;

	FOnWaveEndSignature OnWaveEnd;
};
