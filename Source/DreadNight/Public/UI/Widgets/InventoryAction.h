// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UI/Widgets/Inventory.h"
#include "InventoryAction.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UInventoryAction : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> UseButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> TransferButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> DropButton;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UButton> RemoveButton;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void NativePreConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void OnUsePressed();
	
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
