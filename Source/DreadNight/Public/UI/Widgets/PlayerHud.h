// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

class USmartProgressBar;
class URadialProgressBarImage;
/**
 * 
 */
UCLASS()
class DREADNIGHT_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmartProgressBar> HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmartProgressBar> StaminaBar;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<URadialProgressBarImage> ThirstRadialBarImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<URadialProgressBarImage> HungerRadialBarImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<URadialProgressBarImage> MentalRadialBarImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<URadialProgressBarImage> FearRadialBarImage;

public:
	virtual void NativeConstruct() override;
	
private:

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(const float CurrentValue, const float MaxValue);
	
	UFUNCTION(BlueprintCallable)
	void UpdateStaminaBar(const float CurrentValue, const float MaxValue);

	UFUNCTION(BlueprintCallable)
	void UpdateThirstRadialBarImage(const float CurrentValue);

	UFUNCTION(BlueprintCallable)
	void UpdateHungerRadialBarImage(const float CurrentValue);

	UFUNCTION(BlueprintCallable)
	void UpdateMentalRadialBarImage(const float CurrentValue);

	UFUNCTION(BlueprintCallable)
	void UpdateFearRadialBarImage(const float CurrentValue);
};
