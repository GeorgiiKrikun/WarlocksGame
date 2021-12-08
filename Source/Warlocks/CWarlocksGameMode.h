// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CWarlocksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WARLOCKS_API ACWarlocksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

	void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION()
	void ReactOnDeath();


public:
	void StartPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float _lengthOfInterlude;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float _currentLengthOfInterlude;


private:
	bool _bRespawnGuard = false;



	
};
