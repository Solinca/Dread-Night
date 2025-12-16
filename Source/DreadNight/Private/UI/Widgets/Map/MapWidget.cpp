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

	const FVector ConvertedPlayerLocation = OwnerPawn->GetActorLocation() * MapImageSize / WorldSize;
	PlayerIcon->SetRenderTranslation(FVector2D(ConvertedPlayerLocation));

	const float ConvertedAngle = OwnerPawn->GetActorRotation().Yaw + RotationOffset;
	PlayerIcon->SetRenderTransformAngle(ConvertedAngle);
}
