#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

class UItemNotification;
class UItemInstance;
class UVerticalBox;
class UCompass;
class UTextBlock;
class UHorizontalBox;
class USmartProgressBar;
class URadialProgressBarImage;

UCLASS()
class DREADNIGHT_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmartProgressBar> HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmartProgressBar> StaminaBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmartProgressBar> ManaBar;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<URadialProgressBarImage> ThirstRadialBarImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<URadialProgressBarImage> HungerRadialBarImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<URadialProgressBarImage> MentalRadialBarImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<URadialProgressBarImage> FearRadialBarImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> InformationTextHorizontalBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCompass> PlayerCompass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> NotificationContainer;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(const float CurrentValue, const float MaxValue);
	
	UFUNCTION(BlueprintCallable)
	void UpdateStaminaBar(const float CurrentValue, const float MaxValue);
	
	UFUNCTION(BlueprintCallable)
	void UpdateManaBar(const float CurrentValue, const float MaxValue);

	UFUNCTION(BlueprintCallable)
	void UpdateThirstRadialBarImage(const float CurrentValue, bool IsState);

	UFUNCTION(BlueprintCallable)
	void UpdateHungerRadialBarImage(const float CurrentValue, bool IsState);

	UFUNCTION(BlueprintCallable)
	void UpdateMentalRadialBarImage(const float CurrentValue, bool IsState);

	UFUNCTION(BlueprintCallable)
	void UpdateFearRadialBarImage(const float CurrentValue, bool IsState);
	
	UFUNCTION(BlueprintCallable)
	void UpdateInteractionText(AActor* NewInteractable);
	
	UFUNCTION(BlueprintCallable)
	void AddItemNotification(const UItemInstance* Data, const int Quantity, const TSubclassOf<UItemNotification> WidgetToInstantiate);

protected:
	virtual void NativeConstruct() override;
};
