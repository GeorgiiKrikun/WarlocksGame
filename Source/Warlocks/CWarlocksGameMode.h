// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CWarlocksGameMode.generated.h"

class ACWarlockMainPlayerController;

/**
 * 
 */
UCLASS()
class WARLOCKS_API ACWarlocksGameMode : public AGameMode
{
	GENERATED_BODY()
	

	void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION() 
	void ReactOnDeath();

	void ReactOnDeathBattle();

	void ReactOnDeathInterlude();

	void ReactOnExitInterlude();

	int CheckNumberOfPlayersAlive();

	void RespawnPlayer(ACWarlockMainPlayerController* controller, bool respawnEvenIfAlive = true);

public:
	void StartPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _lengthOfInterlude = 20.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _currentLengthOfInterlude = -1.0f;



	void Tick(float DeltaSeconds) override;

private:
	bool _bRespawnGuard = false;

	

	
protected:
	void BeginPlay() override;

};
