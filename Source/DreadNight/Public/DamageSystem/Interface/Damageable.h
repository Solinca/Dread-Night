#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"
 
UINTERFACE()
class UDamageable : public UInterface
{
	GENERATED_BODY()
};
 
class DREADNIGHT_API IDamageable
{
	GENERATED_BODY()
 
public:
	// Implements this function to check if can apply
	// damage then apply damage inside the function
	virtual bool TryApplyDamage(float Damage,AActor* DamageInstigator);
};
