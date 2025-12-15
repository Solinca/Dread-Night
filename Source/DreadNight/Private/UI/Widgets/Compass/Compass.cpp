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
		FVector CameraForward = PlayerCameraManager->GetCameraRotation().Vector();
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
		FVector ObjectiveLocation = Marker->GetObjectiveActor()->GetActorLocation();

		FVector Direction = ObjectiveLocation - CameraLocation;
		const float Distance = FVector::Dist(ObjectiveLocation, CameraLocation);

		const float ZDifference = ObjectiveLocation.Z - CameraLocation.Z;

		if (ZDifference > 250.f)
		{
			Marker->ShowUpInformationImage();
		}
		else if (ZDifference < 250.f)
		{
			Marker->ShowDownInformationImage();
		}
		else
		{
			Marker->HideAllInformationImage();
		}
			

		CameraForward.Normalize();
		Direction.Normalize();

		const float DotProduct = FVector::DotProduct(CameraForward, Direction);
		const FVector CrossProduct = FVector::CrossProduct(CameraForward, Direction);

		const float Value = CrossProduct.Z > 0.f ? FMath::Acos(DotProduct) : FMath::Acos(DotProduct) * -1.f;
		
		const float TranslationX = UKismetMathLibrary::MapRangeClamped(Value, -180.f, 180.f, -238.f, 238.f);

		Marker->SetRenderTranslation(FVector2D(TranslationX, 0.f));
		FText DistanceText = FText::Format(FText::FromString(TEXT("{0}m")), Distance);
	}
}

float UCompass::GetOffset() const
{
	return GetOwningPlayer()->GetControlRotation().Yaw / 360.f;
}
