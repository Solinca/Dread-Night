// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SmartProgressBar.h"
#include "Components/ProgressBar.h"
#include "TimerManager.h"

void USmartProgressBar::NativeConstruct()
{
    Super::NativeConstruct();

	if (ForegroundBar)
	{
		ForegroundBar->SetPercent(1.0f);
	}
	
	if (GhostBar)
	{
		GhostBar->SetPercent(1.0f);
		GhostBar->SetFillColorAndOpacity(GhostColor);
	}
}

void USmartProgressBar::UpdateValue(const float CurrentValue, const float MaxValue)
{
    if (MaxValue <= 0.0f)
    {
	    return;
    }

	if (!ForegroundBar)
	{
		UE_LOG(LogTemp, Error, TEXT("CRITICAL: SmartProgressBar Widgets are MISSING! Check naming in WBP_SmartBar."));
		return; 
	}
	
	const float NewPercent = FMath::Clamp(CurrentValue / MaxValue, 0.0f, 1.0f);

    ForegroundBar->SetPercent(NewPercent);
    ForegroundBar->SetFillColorAndOpacity(FLinearColor::LerpUsingHSV(EmptyColor, FullColor, NewPercent));

    TargetPercent = NewPercent;

	if (GhostBar)
	{
		if (TargetPercent < GhostBar->GetPercent())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerUpdate);
			GetWorld()->GetTimerManager().SetTimer(TimerDelay, this, &USmartProgressBar::StartGhostAnimation, GhostDelay, false);
			return;
		}
	
		GhostBar->SetPercent(TargetPercent);
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

void USmartProgressBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();

    if (CustomFillMaterial)
    {
        FProgressBarStyle CurrentStyle = ForegroundBar->GetWidgetStyle();

        CurrentStyle.FillImage.SetResourceObject(CustomFillMaterial);
        CurrentStyle.FillImage.ImageSize = FVector2D(32.0f, 32.0f);
        CurrentStyle.FillImage.DrawAs = ESlateBrushDrawType::Box;

        ForegroundBar->SetWidgetStyle(CurrentStyle);
    }

	if (GhostBar)
	{
		GhostBar->SetFillColorAndOpacity(GhostColor);
	}
}

void USmartProgressBar::StartGhostAnimation()
{
    if (GetWorld() && GhostBar)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerUpdate, this, &USmartProgressBar::TickGhostAnimation, 0.016f, true);
    }
}

void USmartProgressBar::TickGhostAnimation()
{
	if (GhostBar)
	{
		const float Current = GhostBar->GetPercent();
		const float Next = FMath::FInterpTo(Current, TargetPercent, 0.016f, GhostSpeed);
    
		GhostBar->SetPercent(Next);

		if (FMath::IsNearlyEqual(Next, TargetPercent, 0.001f))
		{
			GhostBar->SetPercent(TargetPercent);
			GetWorld()->GetTimerManager().ClearTimer(TimerUpdate);
		}
	}
}
