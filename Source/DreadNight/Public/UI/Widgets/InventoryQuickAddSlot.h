#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "InventorySystem/InventoryComponent.h"
#include "UI/Widgets/InventoryQuickAddButton.h"
#include "InventoryQuickAddSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickActionPressedEventSignature, int, SlotIndex);

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
	
public:
	UFUNCTION()
	void SetupMenu(UInventoryComponent* TargetInventory);
	UFUNCTION()
	void OnClicked(int Index);
	
	UVerticalBox* GetVerticalBox() const { return VerticalBox; }
	UInventoryQuickAddButton* GetInventoryQuickAddButton() const { return QuickAddButton; }
	
	FOnQuickActionPressedEventSignature OnQuickActionPressedEvent;
};