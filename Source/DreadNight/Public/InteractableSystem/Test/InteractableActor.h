#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableSystem/Interfaces/Interactable.h"
#include "InteractableActor.generated.h"

class UInteractableComponent;

UCLASS()
class DREADNIGHT_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInteractableComponent> InteractableComponent;
	
public:
	// Sets default values for this actor's properties
	AInteractableActor();

	// Begin IInteractable Interface
	 UFUNCTION(BlueprintCallable)
	virtual void OnInteract_Implementation(AActor* Caller) override;
	virtual void OnFocusGained_Implementation() override;
	virtual void OnFocusLost_Implementation() override;
	virtual FText GetInteractionPromptText_Implementation() override;
	// End IInteractable Interface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
