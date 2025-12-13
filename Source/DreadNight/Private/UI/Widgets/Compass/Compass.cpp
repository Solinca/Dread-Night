// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Compass/Compass.h"

#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widgets/Compass/CompassMarker.h"

void UCompass::AddCompassMarker(AActor* InObjectiveActor)
{
}

void UCompass::NativeConstruct()
{
	Super::NativeConstruct();

	CompassMaterial = CompassImage->GetDynamicMaterial();
	if (CompassMaterial)
	{
		CompassMaterial->SetScalarParameterValue("Offset", GetOffset());
	}
}

void UCompass::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	CompassMaterial->SetScalarParameterValue("Offset", GetOffset());

	UpdateMarkersPosition();
}

void UCompass::UpdateMarkersPosition()
{
	for (const TObjectPtr<UCompassMarker> Marker : MarkersArray)
	{
		const APlayerCameraManager* CameraManager = GetOwningPlayerCameraManager();
		
		FVector CameraForward = CameraManager->GetCameraRotation().Vector();
		FVector CameraLocation = CameraManager->GetCameraLocation();
		FVector ObjectiveLocation = Marker->GetObjectiveActor()->GetActorLocation();

		FVector Direction = ObjectiveLocation - CameraLocation;

		CameraForward.Z = 0.f;
		Direction.Z = 0.f;

		CameraForward.Normalize();
		Direction.Normalize();

		float DotProduct = FVector::DotProduct(CameraForward, Direction);
		FVector CrossProduct = FVector::CrossProduct(CameraForward, Direction);

		float Value = CrossProduct.Z > 0.f ? FMath::Acos(DotProduct) : FMath::Acos(DotProduct) * -1.f;
		
		float TranslationX = UKismetMathLibrary::MapRangeClamped(Value, -180.f, 180.f, -238.f, 238.f);

		Marker->SetRenderTranslation(FVector2D(TranslationX, -20.f));
	}
}

float UCompass::GetOffset() const
{
	return GetOwningPlayer()->GetControlRotation().Yaw / 360.f;
}
