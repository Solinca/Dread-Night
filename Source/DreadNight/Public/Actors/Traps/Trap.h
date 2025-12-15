#pragma once

#include "CoreMinimal.h"
#include "Actors/Building.h"
#include "Components/BoxComponent.h"
#include "IA/Characters/BaseAICharacter.h"
#include "Trap.generated.h"

UCLASS()
class DREADNIGHT_API ATrap : public ABuilding
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UBoxComponent> TrapCollisionComponent;

	/** Potential damage dealt to the monsters. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	float TrapDamage{0.f};

	/** Life span set after the trap has been activated. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	float TrapLifeSpan{0.f};
	
	FTimerHandle TickTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	bool bIsActive = false;
	
	public:
		ATrap();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnTrapTick() {}
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
