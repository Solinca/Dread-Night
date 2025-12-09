#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UsableItem.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UUsableItem : public UInterface
{
	GENERATED_BODY()
};

 
class DREADNIGHT_API IUsableItem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FName GetActionName() = 0;

	//TODO : Replace AActor by the player type.
	virtual void Use(AActor* Player) = 0;
};
