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

	UFUNCTION(Category = Settings)
	float GetMusicVolume() const { return MusicVolume; };

	UFUNCTION(Category = Settings)
	void SetMusicVolume(float value);

	UFUNCTION(Category = Settings)
	float GetSFXVolume() const { return SFXVolume; };

	UFUNCTION(Category = Settings)
	void SetSFXVolume(float value);

	UPROPERTY(GlobalConfig)
	float PlayerCameraSensitivity;

	UPROPERTY(GlobalConfig)
	float MusicVolume;

	UPROPERTY(GlobalConfig)
	float SFXVolume;
};
