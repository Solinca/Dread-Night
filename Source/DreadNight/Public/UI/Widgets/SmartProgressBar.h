// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmartProgressBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class DREADNIGHT_API USmartProgressBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ForegroundBar{ nullptr };

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> GhostBar{ nullptr };
	
	UPROPERTY(EditAnywhere, Category = "Design")
	TObjectPtr<UMaterialInterface> CustomFillMaterial{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Design")
	FLinearColor FullColor{ FLinearColor::Green };

	UPROPERTY(EditAnywhere, Category = "Design")
	FLinearColor EmptyColor{ FLinearColor::Red };
	
	UPROPERTY(EditAnywhere, Category = "Design")
	FLinearColor GhostColor{ FLinearColor(1.0f, 1.0f, 1.0f, 0.5f) };

	UPROPERTY(EditAnywhere, Category = "Design")
	float GhostDelay{ 0.2f };

	UPROPERTY(EditAnywhere, Category = "Design")
	float GhostSpeed{ 5.0f };

private:
	float TargetPercent{ 1.0f };
	FTimerHandle TimerDelay;
	FTimerHandle TimerUpdate;

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Smart Bar")
	void UpdateValue(const float CurrentValue, const float MaxValue);

protected:
	virtual void SynchronizeProperties() override;
	
private:
	void StartGhostAnimation();
	void TickGhostAnimation();
};
