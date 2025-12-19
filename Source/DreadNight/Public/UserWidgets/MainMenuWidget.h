#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Textblock.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

class UOptionsWidget;

UCLASS()
class DREADNIGHT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Logo = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonContinue = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonNewGame = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonOptions = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonQuit = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UOptionsWidget> OptionsWidgetClass;

public:
	UFUNCTION(BlueprintCallable)
	void OnContinueClicked();

	UFUNCTION(BlueprintCallable)
	void OnNewGameClicked();

	UFUNCTION(BlueprintCallable)
	void OnOptionsClicked();

	UFUNCTION(BlueprintCallable)
	void OnQuitClicked();
};