// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BaseAIController.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogAIController, Log, Log)

UCLASS(Abstract, Blueprintable, BlueprintType, Category = "AI", ClassGroup = "AIController")
class DREADNIGHT_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior Tree")
	bool bAutoRunBehaviorTree{true};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behavior Tree")
	TObjectPtr<UBehaviorTree> UsedBehaviorTree;
	
public:
	// Sets default values for this actor's properties
	ABaseAIController();

	void TryRunBehaviorTree();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
};
