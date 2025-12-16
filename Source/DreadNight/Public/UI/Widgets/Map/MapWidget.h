#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

class UImage;


UCLASS()
class DREADNIGHT_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MapImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PlayerIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> RenderTargetMap;

private:
	UPROPERTY(Transient)
	TObjectPtr<APawn> OwnerPawn;

	float RotationOffset{ 0.f };
	float MapImageSize{ 900.f };
	float WorldSize{ 30000.f };

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
