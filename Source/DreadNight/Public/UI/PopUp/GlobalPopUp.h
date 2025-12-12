#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GlobalPopUp.generated.h"

class UTextBlock;

UCLASS()
class DREADNIGHT_API UGlobalPopUp : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TopViewportText;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> ConstructAnimation;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnPopUp(const FText& InText);
};
