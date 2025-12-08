#include "UI/Widgets/PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
void UPauseMenu::NativeConstruct()
{
	if (ResumeButton)
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeButtonClicked);
	if (OptionsButton)
		OptionsButton->OnClicked.AddDynamic(this, &UPauseMenu::OnOptionsButtonClicked);
	if (MenuButton)
		MenuButton->OnClicked.AddDynamic(this, &UPauseMenu::OnMenuButtonClicked);
	if (DesktopButton)
		DesktopButton->OnClicked.AddDynamic(this, &UPauseMenu::OnDesktopButtonClicked);
}

void UPauseMenu::OnResumeButtonClicked()
{
}

void UPauseMenu::OnOptionsButtonClicked()
{
}

void UPauseMenu::OnMenuButtonClicked()
{
}

void UPauseMenu::OnDesktopButtonClicked()
{
	TObjectPtr<APlayerController> PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}
