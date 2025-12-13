#pragma once

#include "CoreMinimal.h"
#include "BTDecorator_Base.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTDecorator_CheckVisibility.generated.h"

struct FBTCheckVisibilityDecoratorMemory
{
	TWeakObjectPtr<AActor> TargetActor;
	float TickInterval{0.f};

	bool LineTraceDidNotTouchedTheTarget{false};
	
	bool bInitialized{false};
};

UCLASS()
class DREADNIGHT_API UBTDecorator_CheckVisibility : public UBTDecorator_Base
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FValueOrBBKey_Float TickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel{ECC_Camera};
	
public:
	UBTDecorator_CheckVisibility();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual uint16 GetInstanceMemorySize() const override;
	
private:
	EBlackboardNotificationResult OnTickIntervalKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	EBlackboardNotificationResult OnTargetActorKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);
	
	bool DoLineTrace(const FVector& Start, const FVector& End, const AActor* TaskOwnerActor, const AActor* RetrievedTargetActor) const;
};
