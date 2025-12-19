#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickedEventSignature);

UCLASS()
class DREADNIGHT_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MenuButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DesktopButton;

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable)
	FOnButtonClickedEventSignature OnResume;
	
	UPROPERTY(BlueprintAssignable)
	FOnButtonClickedEventSignature OnOptions;
	
	UPROPERTY(BlueprintAssignable)
	FOnButtonClickedEventSignature OnQuitToMenu;
	
	UPROPERTY(BlueprintAssignable)
	FOnButtonClickedEventSignature OnQuitToDesktop;

private:
	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnOptionsButtonClicked();

	UFUNCTION()
	void OnMenuButtonClicked();

	UFUNCTION()
	void OnDesktopButtonClicked();
};
