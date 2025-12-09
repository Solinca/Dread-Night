#include "InteractableSystem/Subsystems/InteractableSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "InteractableSystem/Interfaces/Interactable.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

void UInteractableSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LastFocusedActor = nullptr;
}

void UInteractableSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	InWorld.GetTimerManager().SetTimer(IntervalTimerHandle, this, &ThisClass::SearchInteractable, CheckInterval, true);
}

void UInteractableSubsystem::Deinitialize()
{
	Super::Deinitialize();

	GetWorld()->GetTimerManager().ClearTimer(IntervalTimerHandle);
}

bool UInteractableSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}
	const UWorld* World = Cast<const UWorld>(Outer);
	if (!World)
	{
		return false;
	}
	
	if (!World->IsGameWorld())
	{
		return false;
	}

	FString MapName = World->GetName();
	
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	if (MapName.Contains(TEXT("Menu")))
	{
		return false;
	}

	return true;

}

AActor* UInteractableSubsystem::PerformAccurateRayCast(const UWorld* World, const FVector& Start, const FVector& End) const
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerController->GetPawn());
        
	FHitResult HitResult;
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult, 
		Start, 
		End, 
		ECC_Visibility, 
		Params
	);
        
	if (AActor* HitActor = HitResult.GetActor(); bHit && HitActor && HitActor->Implements<UInteractable>())
	{
		DrawDebugLine(World, Start, HitResult.ImpactPoint, FColor::Green, false, 1.f);
		return HitActor;
	}
	return nullptr;
}

AActor* UInteractableSubsystem::PerformPermissiveRayCast(const UWorld* World, const FVector& Start, const FVector& End) const
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerController->GetPawn());
        
	FHitResult HitResult;
	const bool bHit = World->SweepSingleByChannel(
				HitResult,
				Start,
				End,
				FQuat::Identity,
				ECC_Visibility,
				FCollisionShape::MakeSphere(AimToleranceRadius),
				Params
				);
	
	if (AActor* HitActor = HitResult.GetActor(); bHit && HitActor && HitActor->Implements<UInteractable>())
	{
		DrawDebugSphere(World, HitResult.ImpactPoint, AimToleranceRadius, 12, FColor::Yellow, false, 1.f);
		return HitActor;
	}
	return nullptr;
}

void UInteractableSubsystem::SearchInteractable()
{
    AActor* OldActor = LastFocusedActor.Get();
    AActor* NewTargetActor = nullptr;
    
    if (ForcedInteractable.IsValid())
    {
        NewTargetActor = ForcedInteractable.Get();
		CallFocusChanged(NewTargetActor, OldActor);
    	return;
    }

    if (const UWorld* World = GetWorld(); World && PlayerController.Get())
    {
	    FVector CameraLocation;
	    FRotator CameraRotation;
	    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	    const FVector Start = CameraLocation;
	    const FVector End = Start + (CameraRotation.Vector() * InteractionDistance); 

	    NewTargetActor = PerformAccurateRayCast(World, Start, End);

	    if (!NewTargetActor)
	    {
	    	NewTargetActor = PerformPermissiveRayCast(World, Start, End);
	    }
    }
    
    CallFocusChanged(NewTargetActor, OldActor);
}

bool UInteractableSubsystem::TryInteract() const
{
	if (const AActor* Target = LastFocusedActor.Get(); Target && Target->Implements<UInteractable>())
	{
		return true;
	}
	return false;
}

void UInteractableSubsystem::RequestInteraction(AActor* Target, APawn* Instigator) const
{
	IInteractable::Execute_OnInteract(Target, Instigator);
    OnFocusChanged.Broadcast(nullptr);
}

void UInteractableSubsystem::CallFocusChanged(AActor* NewTargetActor, AActor* OldTargetActor)
{
    if (NewTargetActor != OldTargetActor)
    {
        if (OldTargetActor && OldTargetActor->Implements<UInteractable>())
        {
            IInteractable::Execute_OnFocusLost(OldTargetActor);
        }

        if (NewTargetActor && NewTargetActor->Implements<UInteractable>())
        {
            IInteractable::Execute_OnFocusGained(NewTargetActor);
        }

        LastFocusedActor = NewTargetActor;
        
        if (OnFocusChanged.IsBound())
        {
            OnFocusChanged.Broadcast(NewTargetActor);
        }
    }
}
