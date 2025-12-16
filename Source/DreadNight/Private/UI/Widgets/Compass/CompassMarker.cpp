#include "UI/Widgets/Compass/CompassMarker.h"

#include "Components/Image.h"

void UCompassMarker::ShowUpInformationImage() const
{
	DownIconInformation->SetVisibility(ESlateVisibility::Hidden);
	UpIconInformation->SetVisibility(ESlateVisibility::Visible);
}

void UCompassMarker::ShowDownInformationImage() const
{
	UpIconInformation->SetVisibility(ESlateVisibility::Hidden);
	DownIconInformation->SetVisibility(ESlateVisibility::Visible);
}

void UCompassMarker::HideAllInformationImage() const
{
	DownIconInformation->SetVisibility(ESlateVisibility::Hidden);
	UpIconInformation->SetVisibility(ESlateVisibility::Hidden);
}
