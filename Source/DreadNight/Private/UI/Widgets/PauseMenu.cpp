#include "UI/Widgets/PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenu::NativeConstruct()
{
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeButtonClicked);

		ResumeButton->OnHovered.AddDynamic(this, &UPauseMenu::OnButtonHovered);
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UPauseMenu::OnOptionsButtonClicked);

		OptionsButton->OnHovered.AddDynamic(this, &UPauseMenu::OnButtonHovered);
	}

	if (MenuButton)
	{
		MenuButton->OnClicked.AddDynamic(this, &UPauseMenu::OnMenuButtonClicked);

		MenuButton->OnHovered.AddDynamic(this, &UPauseMenu::OnButtonHovered);
	}

	if (DesktopButton)
	{
		DesktopButton->OnClicked.AddDynamic(this, &UPauseMenu::OnDesktopButtonClicked);

		DesktopButton->OnHovered.AddDynamic(this, &UPauseMenu::OnButtonHovered);
	}
}

void UPauseMenu::OnResumeButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), UISelectSound);
	
	OnResume.Broadcast();
}

void UPauseMenu::OnOptionsButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), UISelectSound);

	OnOptions.Broadcast();
}

void UPauseMenu::OnMenuButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), UISelectSound);

	OnQuitToMenu.Broadcast();
}

void UPauseMenu::OnDesktopButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), UISelectSound);

	OnQuitToDesktop.Broadcast();
}

void UPauseMenu::OnButtonHovered()
{
	UGameplayStatics::PlaySound2D(GetWorld(), UIHoverSound);
}
