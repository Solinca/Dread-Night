#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemNotification.generated.h"

class USizeBox;
class UItemInstance;
class UTextBlock;
class UImage;
class UWidgetAnimation;

UCLASS()
class DREADNIGHT_API UItemNotification : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitNotification(const UItemInstance* Data, const int Quantity);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextQuantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageIcon;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnim;

private:
	FTimerHandle TimerHandleLifeSpan;

protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void StartFadeOut();

	UFUNCTION()
	void OnFadeOutFinished();
};
