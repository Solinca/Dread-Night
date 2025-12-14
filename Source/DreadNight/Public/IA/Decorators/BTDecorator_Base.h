// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTDecorator_Base.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UBTDecorator_Base : public UBTDecorator
{
	GENERATED_BODY()

protected:
	/**
	 * The way Unreal works in Behavior Trees is by using 'Node Memory'.
	 * Node Memory is a simple data structure allocated on the heap by the engine for every AI agent using the decorator.
	 * GetInstanceMemorySize returns the size of FBTIsActorMovingDecoratorMemory to allocate.
	 * This way, the engine can use one single decorator instance for multiple AI agents by only changing the node memory address.
	 * 
	 * It optimises memory usage by avoiding the allocation of multiple decorators per AI, which would consume more memory.
	 * Note that you are responsible for managing the values within the Node Memory the engine only handles its allocation and deallocation.
	 *
	 * Unreal store the node memory in uint8 pointer for genericity, and then you cast it to your data struct.
	 *
	 * And Blueprint doesn't use the node memory pattern and instead instantiate one decorator for every IA agent.
	 */
	template<typename T>
	T* CastNodeMemory(uint8* NodeMemory) const
	{
		return reinterpret_cast<T*>(NodeMemory);
	}

	template<typename T>
	T* CastNodeMemory(const UBlackboardComponent* BlackboardComponent) const
	{
		UBehaviorTreeComponent* BehaviorTreeComponent{Cast<UBehaviorTreeComponent>(BlackboardComponent->GetBrainComponent())};
		uint8* NodeMemory{BehaviorTreeComponent->GetNodeMemory(this, BehaviorTreeComponent->FindInstanceContainingNode(this))};
		return reinterpret_cast<T*>(NodeMemory);
	}

	template<typename VBTy_, typename UOTy_, typename MFTy_>
	requires(
		std::derived_from<std::remove_cvref_t<VBTy_>, FValueOrBlackboardKeyBase> &&
		std::derived_from<std::remove_pointer_t<UOTy_>, UObject> &&
		std::is_member_function_pointer_v<MFTy_>
	)
	void RegisterToKeyIdChecked(VBTy_& ValueOrBlackboardKeyBase,
		UBehaviorTreeComponent& BehaviorTreeComponent, UBlackboardComponent* BlackboardComponent, UOTy_* UserObject, MFTy_ MemberFunction)
	{
		const FBlackboard::FKey KeyId{ValueOrBlackboardKeyBase.GetKeyId(BehaviorTreeComponent)};
		if (KeyId != FBlackboard::InvalidKey)
		{
			BlackboardComponent->RegisterObserver(KeyId, UserObject,
				FOnBlackboardChangeNotification::CreateUObject(UserObject, MemberFunction));
		}
	}
};
