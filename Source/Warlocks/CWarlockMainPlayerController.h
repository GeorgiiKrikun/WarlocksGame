// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CWarlockMainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WARLOCKS_API ACWarlockMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void callOnPossess();

	UFUNCTION(BlueprintNativeEvent)
	void OnPossessClient();

	
};
