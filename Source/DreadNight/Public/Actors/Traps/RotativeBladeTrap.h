#pragma once

#include "CoreMinimal.h"
#include "Actors/Traps/Trap.h"
#include "Components/BoxComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Data/Traps/TrapDataAsset.h"
#include "RotativeBladeTrap.generated.h"

UCLASS()
class DREADNIGHT_API ARotativeBladeTrap : public ATrap
{
	GENERATED_BODY()

public:
	ARotativeBladeTrap();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<URotatingMovementComponent> RotatingComponent;

private:
	UFUNCTION()
	void OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
