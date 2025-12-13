// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTTask_Base.generated.h"

/**
 * 
 */
UCLASS()
class DREADNIGHT_API UBTTask_Base : public UBTTaskNode
{
	GENERATED_BODY()

protected:
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
