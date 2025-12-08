#include "UserWidgets/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonContinue->OnClicked.AddDynamic(this, &UMainMenuWidget::ActionContinue);
	ButtonNewGame->OnClicked.AddDynamic(this, &UMainMenuWidget::ActionNewGame);
	ButtonOptions->OnClicked.AddDynamic(this, &UMainMenuWidget::ActionOptions);
	ButtonQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::ActionQuit);
}

void UMainMenuWidget::ActionContinue()
{

}

void UMainMenuWidget::ActionNewGame()
{

}

void UMainMenuWidget::ActionOptions()
{

}

void UMainMenuWidget::ActionQuit()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}