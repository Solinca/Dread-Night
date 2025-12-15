// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Traps/Trap.h"
#include "Components/BoxComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "RotativeBladeTrap.generated.h"

/**
 * 
 */
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
	TObjectPtr<UStaticMeshComponent> TrapBlade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<URotatingMovementComponent> RotatingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float RotationSpeed = 360.f;//en angle/s

private:

	UFUNCTION()
	void OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
