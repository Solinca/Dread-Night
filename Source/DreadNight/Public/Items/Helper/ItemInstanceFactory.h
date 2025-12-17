#pragma once

#include "CoreMinimal.h"
#include "ItemInstanceFactory.generated.h"

class UItemDataAsset;
class UItemInstance;

UCLASS(BlueprintType)
class DREADNIGHT_API UItemInstanceFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static UItemInstance* CreateItem(UObject* Outer,UItemDataAsset* ItemDataAsset, const int StartStackSize);
};
 
