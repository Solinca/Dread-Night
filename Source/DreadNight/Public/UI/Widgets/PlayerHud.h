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
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmartProgressBar> HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USmartProgressBar> StaminaBar;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<URadialProgressBarImage> HungerRadialBarImage;
	
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
	void UpdateHungerRadialBarImage(const float CurrentValue);
	
	UFUNCTION(BlueprintCallable)
	void UpdateInteractionText(AActor* NewInteractable);
	
	UFUNCTION(BlueprintCallable)
	void AddItemNotification(const UItemInstance* Data, const int Quantity, const TSubclassOf<UItemNotification> WidgetToInstantiate);
};
