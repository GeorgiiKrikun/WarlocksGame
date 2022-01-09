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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPawnDeath);

public:
	UFUNCTION(Client, Reliable)
	void callOnPawnRestartClient();

	UFUNCTION(BlueprintNativeEvent)
	void OnPawnRestartClient();

	FOnPawnDeath OnPawnDeath;

	UFUNCTION(Client, Reliable)
	void callOnInterludeBegin(const FString& text);

	UFUNCTION(BlueprintNativeEvent)
	void onInterludeBegin(const FString& text);

	UFUNCTION(Client, Reliable)
	void callOnInterludeEnd();

	UFUNCTION(BlueprintNativeEvent)
	void onInterludeEnd();


protected:
	void OnPossess(APawn* aPawn) override;

	void OnUnPossess() override;

private:
	UFUNCTION()
	void reactOnPawnDeath();

};
