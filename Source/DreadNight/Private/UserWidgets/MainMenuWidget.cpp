#include "UserWidgets/MainMenuWidget.h"
#include "Global/MyGameInstance.h"
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
	if (GetWorld()->GetTimerManager().IsTimerActive(SwitchLevel))
	{
		return;
	}

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(0, 1, 1, FColor::Black, true, true);

	GetWorld()->GetTimerManager().SetTimer(SwitchLevel, [this]
	{
		UGameplayStatics::OpenLevel(this, TEXT("BaseLevel"));
	}, 1, false);
}

void UMainMenuWidget::OnNewGameClicked()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SwitchLevel))
	{
		return;
	}

	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->NewGame();
	}

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(0, 1, 1, FColor::Black, true, true);

	GetWorld()->GetTimerManager().SetTimer(SwitchLevel, [this]
	{
		UGameplayStatics::OpenLevel(this, TEXT("BaseLevel"));
	}, 1, false);
}

void UMainMenuWidget::OnOptionsClicked()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SwitchLevel))
	{
		return;
	}

	RemoveFromParent();

	if (!OptionsWidgetClass)
	{
		return;
	}

	if (UOptionsWidget* OW = CreateWidget<UOptionsWidget>(GetWorld(), OptionsWidgetClass))
	{
		OW->AddToViewport();

		OW->SetMainMenuWidgetClass(MainMenuWidgetClass);
	}
}

void UMainMenuWidget::OnQuitClicked()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SwitchLevel))
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}