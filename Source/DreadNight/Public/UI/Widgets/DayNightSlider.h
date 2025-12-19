#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DayNightSlider.generated.h"

class USlider;


UCLASS()
class DREADNIGHT_API UDayNightSlider : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> DayNightSlider{ nullptr };

private:
	float CurrentDayTime{ 0.f };
	float FullDayTime{ 0.f };
	bool bIsDayCycle{ true };

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void StartDayCycle();
	UFUNCTION()
	void NightStart() { bIsDayCycle = false; }
	
};
