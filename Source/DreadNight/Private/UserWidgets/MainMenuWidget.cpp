#include "UserWidgets/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
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