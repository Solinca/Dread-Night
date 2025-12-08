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

void UInteractableSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SearchInteractable(DeltaTime);
}

TStatId UInteractableSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTargetingSubsystem, STATGROUP_Tickables);
}

void UInteractableSubsystem::SearchInteractable(const float DeltaTime)
{
    TimeSinceLastCheck += DeltaTime;
	
    if (TimeSinceLastCheck < CheckInterval)
    {
    	return;
    }
	
    TimeSinceLastCheck = 0.0f;
    AActor* NewTargetActor = nullptr;

    if (ForcedInteractable.IsValid())
    {
        NewTargetActor = ForcedInteractable.Get();
    }
    else
    {
        const UWorld* World = GetWorld();
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);

        if (World && PlayerController)
        {
            FVector CameraLocation;
            FRotator CameraRotation;
            PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

            const FVector Start = CameraLocation;
            const FVector End = Start + (CameraRotation.Vector() * InteractionDistance); 

            FCollisionQueryParams Params;
            Params.AddIgnoredActor(PlayerController->GetPawn());
            
            FHitResult HitResult;
            bool bHit = World->LineTraceSingleByChannel(
                HitResult, 
                Start, 
                End, 
                ECC_Visibility, 
                Params
            );
            
            AActor* HitActor = HitResult.GetActor();

            if (bHit && HitActor && HitActor->Implements<UInteractable>())
            {
                NewTargetActor = HitActor;
                DrawDebugLine(World, Start, HitResult.ImpactPoint, FColor::Green, false, 1.f);
            }
            else 
            {
                bHit = World->SweepSingleByChannel(
                    HitResult,
                    Start,
                    End,
                    FQuat::Identity,
                    ECC_Visibility,
                    FCollisionShape::MakeSphere(AimToleranceRadius),
                    Params
                );

                HitActor = HitResult.GetActor();

                if (bHit && HitActor && HitActor->Implements<UInteractable>())
                {
                    NewTargetActor = HitActor;
                    DrawDebugSphere(World, HitResult.ImpactPoint, AimToleranceRadius, 12, FColor::Yellow, false, 1.f);
                }
            }
        }
    }

    AActor* OldActor = LastFocusedActor.Get();
    if (NewTargetActor != OldActor)
    {
        if (OldActor && OldActor->Implements<UInteractable>())
        {
            IInteractable::Execute_OnFocusLost(OldActor);
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