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
	TWeakObjectPtr<AActor> LastFocusedActor{ nullptr };
	const float AimToleranceRadius = 10.0f;
	const float CheckInterval{ 0.05f };
	const float InteractionDistance{ 200.0f };
	float TimeSinceLastCheck{ 0.f };
	TWeakObjectPtr<AActor> ForcedInteractable{ nullptr };
	
public:
	// Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// End USubsystem Interface

	// Begin UTickableWorldSubsystem Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// End UTickableWorldSubsystem Interface

	AActor* GetLastFocusedActor() const { return LastFocusedActor.Get(); }
	void SearchInteractable(const float DeltaTime);
	bool TryInteract() const;
	void RequestInteraction(AActor* Target, APawn* Instigator) const;
};
