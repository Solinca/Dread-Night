// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/InventorySlot.h"

void UInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventorySlot::SetItemImage(UTexture2D* Texture)
{
	ItemImage->SetBrushFromTexture(Texture);
}

void UInventorySlot::SetStackText(int Stack)
{
	StackText->SetText(FText::FromString(FString::FromInt(Stack)));
}
