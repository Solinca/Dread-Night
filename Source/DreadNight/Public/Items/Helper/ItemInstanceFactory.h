#pragma once

#include "CoreMinimal.h"

class UItemDataAsset;
class UItemInstance;


class DREADNIGHT_API FItemInstanceFactory
{
public:
	static UItemInstance* CreateItem(UObject* Outer,UItemDataAsset* ItemDataAsset, const int StartStackSize);
};
