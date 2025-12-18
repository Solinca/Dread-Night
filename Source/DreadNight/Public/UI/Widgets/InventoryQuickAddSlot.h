#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "UI/Widgets/InventoryQuickAddButton.h"
#include "InventoryQuickAddSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickActionPressedEventSignature);

UCLASS()
class DREADNIGHT_API UInventoryQuickAddSlot : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UVerticalBox> VerticalBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryQuickAddButton> QuickAddButtonClass;
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION()
	void SetSize(int Size);
	UFUNCTION()
	void OnClicked();
		
	TObjectPtr<UVerticalBox> GetVerticalBox() const { return VerticalBox; }
	
	FOnQuickActionPressedEventSignature OnQuickActionPressedEvent;
};