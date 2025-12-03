#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftStationWidget.generated.h"

class UTextBlock;

UCLASS()
class DREADNIGHT_API UCraftStationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StationName;
};
