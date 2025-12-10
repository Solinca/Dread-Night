// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Textblock.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UImage> ItemImage;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> StackText;
public:
	UFUNCTION(BlueprintCallable)
	virtual void NativePreConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetItemImage(UTexture2D* Texture);
	UFUNCTION(BlueprintCallable)
	void SetStackText(int Stack);
};
