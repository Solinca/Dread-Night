#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Compass.generated.h"


class UOverlay;
class UCompassMarker;
class UImage;

UCLASS()
class DREADNIGHT_API UCompass : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CompassImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay;
	
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> CompassMaterial;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UCompassMarker>> MarkersArray;
	
	UPROPERTY(Transient)
	TObjectPtr<APlayerCameraManager> PlayerCameraManager;

public:
	UFUNCTION(BlueprintCallable)
	void AddCompassMarker(UCompassMarker* InCompassMarker);
	UFUNCTION(BlueprintCallable)
	void RemoveCompassMaker(UCompassMarker* OutCompassMarker);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdateMarkersPosition();
	float GetOffset() const;
};
