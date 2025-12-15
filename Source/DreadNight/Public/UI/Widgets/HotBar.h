#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Inventory.h"
#include "HotBar.generated.h"

class UHotBarSlot;
class UHorizontalBox;

UCLASS()
class DREADNIGHT_API UHotBar : public UInventory
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
};
