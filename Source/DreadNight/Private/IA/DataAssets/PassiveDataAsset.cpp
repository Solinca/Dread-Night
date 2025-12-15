#include "IA/DataAssets/PassiveDataAsset.h"

float UPassiveDataAsset::GetFleeTime() const
{
	return FleeTime;
}

float UPassiveDataAsset::GetFleeingSpeed() const
{
	return FleeingSpeed;
}

float UPassiveDataAsset::GetFleeingAcceptableRadius() const
{
	return FleeingAcceptableRadius;
}

float UPassiveDataAsset::GetFleeingWaitTime() const
{
	return FleeingWaitTime;
}

UEnvQuery* UPassiveDataAsset::GetFleeingEQS() const
{
	return FleeingEQS;
}

float UPassiveDataAsset::GetWanderingSpeed() const
{
	return WanderingSpeed;
}

UEnvQuery* UPassiveDataAsset::GetWanderEQS() const
{
	return WanderEQS;
}

float UPassiveDataAsset::GetWanderAcceptableRadius() const
{
	return WanderAcceptableRadius;
}

float UPassiveDataAsset::GetWanderWaitTime() const
{
	return WanderWaitTime;
}
