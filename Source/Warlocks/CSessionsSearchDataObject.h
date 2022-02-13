// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSessionsSearchDataObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class WARLOCKS_API UCSessionsSearchDataObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FString GetSessionName();

	UFUNCTION(BlueprintCallable)
	int GetNumPlayers();

	UFUNCTION(BlueprintCallable)
	int GetNumMaxPlayers();
	
};
