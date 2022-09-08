// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CWarlockChar.h"
#include "GameFramework/GameState.h"
#include "CGameStateBase.generated.h"

/**
 * 
 */

UCLASS()
class WARLOCKS_API ACGameStateBase : public AGameState
{
	GENERATED_BODY()

public:

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float _thresholdValue;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool _areWeInMatch = true;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<ACWarlockChar*>  _characters;


	
};
