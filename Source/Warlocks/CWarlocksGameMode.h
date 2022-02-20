// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Templates/SubclassOf.h"
#include "CAIController.h"
#include "CWarlockChar.h"
#include "CWarlocksGameMode.generated.h"

class ACWarlockMainPlayerController;

/**
 * 
 */

USTRUCT() struct FDamageStatistics {
	GENERATED_BODY()

	UPROPERTY()
	TMap<APlayerController*, float> _damageDoneMap;

	FString pop();

};

UCLASS()
class WARLOCKS_API ACWarlocksGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACWarlocksGameMode();

	void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION() 
	void ReactOnDeath();

	void ReactOnDeathBattle();

	void ReactOnDeathInterlude();

	void ReactOnEnterInterlude();

	void ReactOnExitInterlude();

	void AwardCoinsAccordingToStats();

	int CheckNumberOfPlayersAlive();

	void RespawnPlayer(ACWarlockMainPlayerController* controller, bool respawnEvenIfAlive = true, bool atSpecifiedLocation = false, FVector location = FVector(0,0,0), FRotator rotation = FRotator(0,0,0));

public:
	void StartPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _lengthOfInterlude = 20.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _currentLengthOfInterlude = -1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _DamageForCoin = 30.0f;
	// Arena size
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _minThreshold = 100.0f;

	// Arena size
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _beginThreshold = 2000.0f;

	// Arena size
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _thresholdVelocityDecrease = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _currentTreshold = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _respawnDelay = 1.0f;


	UFUNCTION(BlueprintCallable, Exec)
	void addBot();

	// Bot controller class
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACAIController> _botControllerClass;

	// Character Class for bot
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACWarlockChar> _actualClassToSpawn;

	void addDamageStatisticsEntry(ACWarlockMainPlayerController* from, float damage);

	void Tick(float DeltaSeconds) override;



private:
	bool _bRespawnGuard = false;


	UPROPERTY()
	FDamageStatistics _damageStatistics;

	

	
protected:
	void BeginPlay() override;

};
