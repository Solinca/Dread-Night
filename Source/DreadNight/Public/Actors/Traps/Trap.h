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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (EditCondition = "!bTrapIsUsingTimer && !bTrapTickInfinitely"))
	int32 TrapTickCount{0};
	int32 TrapTickCurrentCount{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (EditCondition = "bTrapIsUsingTimer && !bTrapTickInfinitely"))
	float TrapTimer{0.f};

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variable", meta = (AllowPrivateAccess = true))
	bool bTrapTickInfinitely{false};

	/**
	 * Determines the trap's ticking behavior:
	 * - false: The trap ticks at regular intervals defined by TickInterval (e.g., every second) for a set number of iterations, 
	 *   then automatically stops by calling StopTrapTicking.
	 * - true: The trap ticks every frame for a duration specified by TrapTimer, then automatically stops by calling StopTrapTicking.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variable", meta = (EditCondition = "!bTrapTickInfinitely", AllowPrivateAccess = true))
	bool bTrapIsUsingTimer{false};
	
	FTimerHandle TickTimerHandle;
	
public:
	ATrap();

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * This function is used to start the ticking process of the trap.
	 * After this function is called OnTrapTick will start to be called.
	 */
	void StartTrapTicking();

	/**
	 * This function is used to stop the ticking process of the trap.
	 * When this function is called OnTrapTickFinish will be called.
	 */
	void StopTrapTicking();

	virtual void OnTrapTick(float DeltaSeconds) {}

	virtual void OnTrapTickFinish() {}

	virtual void OnTrapTrigger(ABaseAICharacter* TrapTriggerInvestigator) {}
private:
	void CheckToFinishTrapTicking();
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
