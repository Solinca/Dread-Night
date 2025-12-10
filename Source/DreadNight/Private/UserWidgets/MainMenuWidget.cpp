#include "UserWidgets/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserWidgets/OptionsWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonContinue->OnClicked.AddDynamic(this, &UMainMenuWidget::OnContinueClicked);
	ButtonNewGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
	ButtonOptions->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
	ButtonQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
}

void UMainMenuWidget::OnContinueClicked()
{
	// TODO: Load save if exists or disable button if not
}

void UMainMenuWidget::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(this, TEXT("BaseLevel"));
}

void UMainMenuWidget::OnOptionsClicked()
{
	RemoveFromParent();

	if (!OptionsWidgetClass) return;

	if (UOptionsWidget* OW = CreateWidget<UOptionsWidget>(GetWorld(), OptionsWidgetClass))
	{
		OW->AddToViewport();
		OW->SetMainMenuWidgetClass(MainMenuWidgetClass);
	}
}

void UMainMenuWidget::OnQuitClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}