#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Inventory.h"
#include "HotBar.generated.h"

class UHotBarSlot;
class UHorizontalBox;

UCLASS()
class DREADNIGHT_API UHotBar : public UInventory
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UImage> HelmetImage;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UImage> ChestImage;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UImage> PantImage;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UImage> ShoesImage;
	
public:
	virtual void NativeConstruct() override;
	
	UImage* GetHelmetImage() const { return HelmetImage; }
	UImage* GetChestImage() const { return ChestImage; }
	UImage* GetPantImage() const { return PantImage; }
	UImage* GetShoesImage() const { return ShoesImage; }
	
	void SetHelmetBrush(UTexture2D* HelmetTexture) const;
	void SetChestBrush(UTexture2D* ChestTexture) const;
	void SetPantBrush(UTexture2D* PantTexture) const;
	void SetShoesBrush(UTexture2D* ShoesTexture) const;
	
	void SetArmorImagesVisibility(ESlateVisibility ArmorImagesVisibility) const;
};
