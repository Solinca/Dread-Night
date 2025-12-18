#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "MyGameUserSettings.generated.h"

UCLASS(config = GameUserSettings, configdonotcheckdefaults)
class DREADNIGHT_API UMyGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UMyGameUserSettings(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(Category = Settings)
	float GetPlayerCameraSensitivity() const { return PlayerCameraSensitivity; };

	UFUNCTION(Category = Settings)
	void SetPlayerCameraSensitivity(float value);

	UPROPERTY(GlobalConfig)
	float PlayerCameraSensitivity;
};
