#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftStationWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class DREADNIGHT_API UCraftStationWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StationName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CraftButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> QuitButton;

protected:
	UFUNCTION()
	void OnCraftButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();
};
