#include "UI/Widgets/Map/MapWidget.h"

#include "Components/Image.h"

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->SpawnActor<AActor>(RenderTargetMap, FVector(0.f,0.f, 5000.f), FRotator(-90, 0, 0));

	OwnerPawn = GetOwningPlayerPawn();
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float Ratio = MapImageSize / WorldSize;
	const FVector PlayerLocation = OwnerPawn->GetActorLocation();

	const float ConvertedX = PlayerLocation.Y * Ratio; 

	const float ConvertedY = -PlayerLocation.X * Ratio; 

	PlayerIcon->SetRenderTranslation(FVector2D(ConvertedX, ConvertedY));

	const float ConvertedAngle = OwnerPawn->GetActorRotation().Yaw + RotationOffset;
	PlayerIcon->SetRenderTransformAngle(ConvertedAngle);
}
