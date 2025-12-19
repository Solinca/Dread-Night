#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "InventorySystem/InventoryComponent.h"
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
	
	UPROPERTY()
	TObjectPtr<UInventoryQuickAddButton> QuickAddButton;
	
	UPROPERTY()
	TObjectPtr<UInventoryComponent> TargetInventory;
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION()
	void SetupMenu(UInventoryComponent* Inventory);
	UFUNCTION()
	void OnClicked();
		
	TObjectPtr<UVerticalBox> GetVerticalBox() const { return VerticalBox; }
	TObjectPtr<UInventoryQuickAddButton> GetInventoryQuickAddButton() const { return QuickAddButton; }
	
	FOnQuickActionPressedEventSignature OnQuickActionPressedEvent;
};