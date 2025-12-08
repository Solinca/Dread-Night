#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "InteractableSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusChangedSignature, AActor*, NewInteractable);

UCLASS()
class DREADNIGHT_API UInteractableSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnFocusChangedSignature OnFocusChanged;
	
private:
	TWeakObjectPtr<APlayerController> PlayerController;
	TWeakObjectPtr<AActor> LastFocusedActor{ nullptr };
	TWeakObjectPtr<AActor> ForcedInteractable{ nullptr };
	FTimerHandle IntervalTimerHandle;
	const float AimToleranceRadius{ 10.0f };
	const float CheckInterval{ 0.05f };
	const float InteractionDistance{ 200.0f };
	
public:
	// Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// End USubsystem Interface

	// Begin UTickableWorldSubsystem Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// End UTickableWorldSubsystem Interface

	AActor* GetLastFocusedActor() const { return LastFocusedActor.Get(); }

	AActor* PerformAccurateRayCast(const UWorld* World, const FVector& Start, const FVector& End) const ;
	AActor* PerformPermissiveRayCast(const UWorld* World, const FVector& Start, const FVector& End) const;
	void SearchInteractable();


	
	bool TryInteract() const;
	UFUNCTION(BlueprintCallable)
	void RequestInteraction(AActor* Target, APawn* Instigator) const;
	void CallFocusChanged(AActor* NewTargetActor, AActor* OldTargetActor);
};
