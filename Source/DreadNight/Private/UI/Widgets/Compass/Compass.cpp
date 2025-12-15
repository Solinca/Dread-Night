// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Compass/Compass.h"

#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widgets/Compass/CompassMarker.h"

void UCompass::AddCompassMarker(UCompassMarker* InCompassMarker)
{
	MarkersArray.Add(InCompassMarker);
}

void UCompass::RemoveCompassMaker(UCompassMarker* OutCompassMarker)
{
	MarkersArray.Remove(OutCompassMarker);
	OutCompassMarker->RemoveFromParent();
}

void UCompass::NativeConstruct()
{
	Super::NativeConstruct();

	CompassMaterial = CompassImage->GetDynamicMaterial();
	if (CompassMaterial)
	{
		CompassMaterial->SetScalarParameterValue("Offset", GetOffset());
	}
	PlayerCameraManager = GetOwningPlayerCameraManager();
	OutRangeA = -CompassImage->GetDesiredSize().X / 2.f; // HalfSize, needs to be in negative
	OutRangeB = CompassImage->GetDesiredSize().X / 2.f; // HalfSize
}

void UCompass::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	CompassMaterial->SetScalarParameterValue("Offset", GetOffset());

	UpdateMarkersPosition();
}

void UCompass::UpdateMarkersPosition()
{
	for (UCompassMarker* Marker : MarkersArray)
	{
		if (!Marker || !Marker->GetObjectiveActor())
		{
			continue;
		}
		
		FVector CameraForward = PlayerCameraManager->GetCameraRotation().Vector();
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
		FVector ObjectiveLocation = Marker->GetObjectiveActor()->GetActorLocation();

		FVector Direction = ObjectiveLocation - CameraLocation;
		const float Distance = Direction.Size();

		const float ZDifference = ObjectiveLocation.Z - CameraLocation.Z;

		if (FMath::Abs(ZDifference) <= VerticalThreshold)
		{
			Marker->HideAllInformationImage();
		}
		else
		{
			(ZDifference > 0.f) ? Marker->ShowUpInformationImage() : Marker->ShowDownInformationImage();
		}
			
		CameraForward.Z = 0.f;
		Direction.Z = 0.f;
		
		CameraForward.Normalize();
		Direction.Normalize();

		const float DotProduct = FVector::DotProduct(CameraForward, Direction);
		const FVector CrossProduct = FVector::CrossProduct(CameraForward, Direction);
		float AngleRadians = FMath::Acos(DotProduct);
		float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
		if (CrossProduct.Z < 0.f)
		{
			AngleDegrees *= -1.f;
		}
		
		const float TranslationX = UKismetMathLibrary::MapRangeClamped(AngleDegrees, InRangeA, InRangeB, OutRangeA, OutRangeB);

		Marker->SetRenderTranslation(FVector2D(TranslationX, 0.f));

		if (Distance != 0.f)
		{
			FText DistanceText = FText::FromString(FString::Printf(TEXT("%.1f m"), Distance / 100.f)); // meters conversion
			Marker->SetDistanceText(DistanceText);
		}
	}
}

float UCompass::GetOffset() const
{
	return GetOwningPlayer()->GetControlRotation().Yaw / 360.f; // just a full turn angle
}
