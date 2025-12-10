// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/Glossary.h"

#include "Components/ListViewBase.h"
#include "Components/RetainerBox.h"

void UGlossary::NativeConstruct()
{
	Super::NativeConstruct();

	if (RetainerBox)
	{
		DynamicMaterial = RetainerBox->GetEffectMaterial();
	}

	OnListScrolled(0.f, 1.f);
}

void UGlossary::OnListScrolled(const float ItemOffset, const float DistanceRemaining) const
{
	if (!DynamicMaterial)
	{
		return;
	}
	
	constexpr float TopThreshold = 2.5f; 
	constexpr float BottomThreshold = 0.1f;

	const float TopStrength = FMath::SmoothStep(0.0f, TopThreshold, ItemOffset);
	DynamicMaterial->SetScalarParameterValue("TopStrength", TopStrength);
    
	const float BottomStrength = FMath::SmoothStep(0.0f, BottomThreshold, DistanceRemaining);
	DynamicMaterial->SetScalarParameterValue("BottomStrength", BottomStrength);
}
