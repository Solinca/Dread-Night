#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class UDN_SaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPCGEndGenerationSignature);

UCLASS()
class DREADNIGHT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	int Seed;

	bool bIsNewGame = false;

protected:
	virtual void Init() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundMix> MusicSoundMix = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundClass> MusicSoundClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundMix> SFXSoundMix = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	TObjectPtr<USoundClass> SFXSoundClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FName SaveSlotName;
	
	UPROPERTY(EditDefaultsOnly)
	int UserIndex;

	UPROPERTY(Transient)
	UDN_SaveGame* SaveGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
	bool bIsFirstLaunch = true;

public:
	UFUNCTION(BlueprintCallable)
	void NewGame();

	UFUNCTION(BlueprintCallable)
	void Save(UWorld* World);

	UFUNCTION(BlueprintCallable)
	void Load(UWorld* World);

	UFUNCTION(BlueprintCallable)
	bool IsNewGame();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPCGEndGenerationSignature OnPCGEndGeneration;

	UFUNCTION(BlueprintCallable)
	int GetSeed();
};
