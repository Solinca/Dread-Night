#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MagicCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREADNIGHT_API UMagicCombatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	UMagicCombatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Combat")
	TSubclassOf<AActor> FireballClass;

public:
	UFUNCTION(BlueprintCallable)
	void CastFireball();
};
