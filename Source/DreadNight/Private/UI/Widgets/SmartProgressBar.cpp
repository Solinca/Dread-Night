#include "UI/Widgets/SmartProgressBar.h"
#include "Components/ProgressBar.h"
#include "TimerManager.h"

void USmartProgressBar::NativeConstruct()
{
    Super::NativeConstruct();

	UpdateValue(1.f, 1.f);
}

void USmartProgressBar::UpdateValue(const float CurrentValue, const float MaxValue)
{
    if (MaxValue <= 0.0f)
    {
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
