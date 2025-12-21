#pragma once

#include "CoreMinimal.h"
#include "Actors/Traps/Trap.h"
#include "RotativeBladeTrap.generated.h"

UCLASS()
class DREADNIGHT_API ARotativeBladeTrap : public ATrap
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	ARotativeBladeTrap();

	virtual void BeginPlay() override;

	virtual void ActivateTrap() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class URotatingMovementComponent> RotatingComponent;
};
