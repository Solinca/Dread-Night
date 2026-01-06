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

	virtual void Tick(float DeltaTime) override;

	virtual void ActivateTrap() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class URotatingMovementComponent> RotatingComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> PillarMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float LerpHeightSpeed = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float FinishSpawnHeight = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float LerpScaleSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Values")
	float FinishScale = 4.f;

private:

	bool AnimateHeight = false;
	float CurrentSpawnHeight;

	bool AnimateScale = false;
	float CurrentScale;

public:

	UFUNCTION()
	void AnimateRotativeBladeTrap(float DeltaTime);

};
