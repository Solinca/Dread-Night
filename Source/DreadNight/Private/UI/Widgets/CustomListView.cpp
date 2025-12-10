// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/CustomListView.h"

#include "Components/ListViewBase.h"
#include "Components/RetainerBox.h"

void UCustomListView::NativeConstruct()
{
	Super::NativeConstruct();

	if (RetainerBox)
	{
		DynamicMaterial = RetainerBox->GetEffectMaterial();
	}

	OnListScrolled(0.f, 1.f);
}

void UCustomListView::OnListScrolled(const float ItemOffset, const float DistanceRemaining) const
{
	if (!DynamicMaterial)
	{
		return;
	}
	
	const float TopStrength = FMath::Clamp(ItemOffset, 0.0f, 1.0f);
	DynamicMaterial->SetScalarParameterValue("TopStrength", TopStrength);
	
	const float BottomStrength = FMath::Clamp(DistanceRemaining, 0.0f, 1.0f);
	DynamicMaterial->SetScalarParameterValue("BottomStrength", BottomStrength);
}
