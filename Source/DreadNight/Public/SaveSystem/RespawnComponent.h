// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RespawnComponent.generated.h"

DECLARE_DELEGATE(FRespawnEventSignature)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DREADNIGHT_API URespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URespawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
	int RespawnNightNumber = 1;

	UPROPERTY(SaveGame)
	int NightBeforeRespawn;

	UFUNCTION()
	void OnNewDayUpdate();

	UPROPERTY(SaveGame)
	bool bIsDead;
public:

	void Despawn();

	void Respawn();

	void OnPostLoad();

	FRespawnEventSignature OnRespawn;
};
