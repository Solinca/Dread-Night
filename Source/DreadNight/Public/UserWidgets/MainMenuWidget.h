#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Textblock.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class DREADNIGHT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TBTitle = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonContinue = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonNewGame = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonOptions = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonQuit = nullptr;
};