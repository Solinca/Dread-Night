#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "OptionsWidget.generated.h"

class UMainMenuWidget;

UCLASS()
class DREADNIGHT_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	TMap<FString, EWindowMode::Type> WindowModeMap;
	TMap<FString, FIntPoint> ResolutionMap;
	TMap<FString, int> GraphicsMap;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TBTitle = nullptr;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBoxWindowMode = nullptr;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBoxResolution = nullptr;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBoxGraphics = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBoxVSync = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonReturn = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnGraphicsChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnCheckboxVSyncChanged(bool bIsChecked);

	UFUNCTION()
	void OnReturnClicked();
};