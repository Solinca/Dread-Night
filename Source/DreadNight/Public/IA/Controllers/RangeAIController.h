#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "RangeAIController.generated.h"

UCLASS(Blueprintable, BlueprintType, Category = "AI", ClassGroup = "AIController")
class DREADNIGHT_API ARangeAIController : public ABaseAIController
{
	GENERATED_BODY()

protected:
	virtual void SetupBlackboard(UBlackboardComponent* BlackboardComponent) override;
};
