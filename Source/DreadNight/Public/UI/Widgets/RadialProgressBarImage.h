// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialProgressBarImage.generated.h"


class UImage;
class UMaterialInstanceDynamic;
/**
 * 
 */
UCLASS()
class DREADNIGHT_API URadialProgressBarImage : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UImage> RadialImage{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Design")
	TObjectPtr<UMaterialInstanceDynamic> RadialMaterialInstance{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Design")
	FColor RadialColor{ FColor::White };

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "Radial Bar")
	void UpdateValue(const float CurrentValue);
};
