// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UButton;

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
