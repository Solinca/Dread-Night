#pragma once

#include "CoreMinimal.h"
#include "Actors/Building.h"
#include "Trap.generated.h"

UCLASS()
class DREADNIGHT_API ATrap : public ABuilding
{
	GENERATED_BODY()
	
public:
	ATrap();

protected:
	virtual void BeginPlay() override;
};
