#pragma once

#include "CoreMinimal.h"
#include "Actors/Building.h"
#include "Components/BoxComponent.h"
#include "IA/Characters/BaseAICharacter.h"
#include "Data/Traps/TrapDataAsset.h"
#include "Trap.generated.h"

UCLASS()
class DREADNIGHT_API ATrap : public ABuilding
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UBoxComponent> TrapCollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UTrapDataAsset> TrapData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	bool bIsActive = false;
	
	public:
		ATrap();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PlaceBuilding() override;

	virtual void OnTrapTick() {}
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
