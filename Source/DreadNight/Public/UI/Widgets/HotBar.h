#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HotBar.generated.h"

class UHotBarSlot;
class UHorizontalBox;

UCLASS()
class DREADNIGHT_API UHotBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> SlotsContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FKey> HotBarKeys;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "4", ClampMax = "10"))
	int32 HotBarSlotCount = 4;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UHotBarSlot> HotBarSlotClass;
	
	TArray<TObjectPtr<UHotBarSlot>> SlotsArray;
	
public:
	virtual void NativeConstruct() override;
};
