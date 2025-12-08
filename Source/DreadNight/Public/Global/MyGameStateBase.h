#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNightStartSignature);

UCLASS()
class DREADNIGHT_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	FOnNightStartSignature OnNightStart;
};
