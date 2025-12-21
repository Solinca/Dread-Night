#pragma once

#include "CoreMinimal.h"
#include "IA/Characters/BaseAICharacter.h"
#include "Trap.generated.h"

UCLASS()
class DREADNIGHT_API ATrap : public AActor
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	ATrap();

	virtual void BeginPlay() override;

	virtual void ActivateTrap() {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UBoxComponent> TrapCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UBuildingDataAsset> TrapData;
};
