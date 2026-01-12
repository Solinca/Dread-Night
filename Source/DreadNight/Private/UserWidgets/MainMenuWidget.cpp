#include "UserWidgets/MainMenuWidget.h"
#include "Global/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UserWidgets/OptionsWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonContinue->OnClicked.AddDynamic(this, &UMainMenuWidget::OnContinueClicked);

	ButtonContinue->OnHovered.AddDynamic(this, &UMainMenuWidget::OnButtonHover);
	
	ButtonNewGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
	
	ButtonNewGame->OnHovered.AddDynamic(this, &UMainMenuWidget::OnButtonHover);
	
	ButtonOptions->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
	
	ButtonOptions->OnHovered.AddDynamic(this, &UMainMenuWidget::OnButtonHover);
	
	ButtonQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	
	ButtonQuit->OnHovered.AddDynamic(this, &UMainMenuWidget::OnButtonHover);

	ButtonContinue->SetVisibility(ESlateVisibility::Visible);
	
	
	if (UMyGameInstance* GameInstance = GetGameInstance<UMyGameInstance>(); GameInstance && !GameInstance->DoesSaveExist())
	{
		ButtonContinue->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void UMainMenuWidget::OnContinueClicked()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SwitchLevel))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), UISelectSound);

	ChangeLevel();
}

void UMainMenuWidget::OnNewGameClicked()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SwitchLevel))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), UISelectSound);

	if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->NewGame();
	}

	ChangeLevel();
}

void UMainMenuWidget::ChangeLevel()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PC->SetShowMouseCursor(false);

	PC->SetInputMode(FInputModeGameOnly());

	PC->SetIgnoreLookInput(true);

	PC->PlayerCameraManager->StartCameraFade(0, 1, 1, FColor::Black, true, true);

	GetWorld()->GetTimerManager().SetTimer(SwitchLevel, [this]
	{
		UGameplayStatics::OpenLevel(this, TEXT("BaseLevel"));
	}, 1, false);
}

void UMainMenuWidget::OnButtonHover()
{
	UGameplayStatics::PlaySound2D(GetWorld(), UIHoverSound);
}

void UMainMenuWidget::OnOptionsClicked()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SwitchLevel))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), UISelectSound);

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

	UGameplayStatics::PlaySound2D(GetWorld(), UISelectSound);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}