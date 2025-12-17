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
	TObjectPtr<UImage> ArmorImage;
	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UImage> WeaponImage;
	
public:
	virtual void NativeConstruct() override;
	
	TObjectPtr<UImage> GetHelmetImage() const { return HelmetImage; }
	TObjectPtr<UImage> GetArmorImage() const { return ArmorImage; }
	TObjectPtr<UImage> GetWeaponImage() const { return WeaponImage; }
	
	void SetHelmetBrush(UTexture2D* HelmetTexture) const;
	void SetArmorBrush(UTexture2D* ArmorTexture) const;
	void SetWeaponBrush(UTexture2D* WeaponTexture) const;
};
