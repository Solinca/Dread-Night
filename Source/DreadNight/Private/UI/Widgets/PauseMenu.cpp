#include "UI/Widgets/PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenu::NativeConstruct()
{
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeButtonClicked);
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UPauseMenu::OnOptionsButtonClicked);
	}

	if (MenuButton)
	{
		MenuButton->OnClicked.AddDynamic(this, &UPauseMenu::OnMenuButtonClicked);
	}

	if (DesktopButton)
	{
		DesktopButton->OnClicked.AddDynamic(this, &UPauseMenu::OnDesktopButtonClicked);
	}
}

void UPauseMenu::OnResumeButtonClicked()
{
	OnResume.Broadcast();
}

void UPauseMenu::OnOptionsButtonClicked()
{
	OnOptions.Broadcast();
}

void UPauseMenu::OnMenuButtonClicked()
{
	OnQuitToMenu.Broadcast();
}

void UPauseMenu::OnDesktopButtonClicked()
{
	OnQuitToDesktop.Broadcast();
}
