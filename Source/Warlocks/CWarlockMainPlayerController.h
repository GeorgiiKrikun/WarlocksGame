// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSubsystem.h"
#include "CWarlockChar.h"
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


// ONLINE SUBSYSTEM INTERFACE
	UFUNCTION(BlueprintCallable)
	bool connectToSteam();
	
	UFUNCTION(BlueprintCallable)
	void createLobby(FName Name = TEXT("HEH"));

	UFUNCTION(BlueprintCallable)
	void scanSessions();



// cheats

	UFUNCTION(BlueprintCallable, Exec)
	void lookAtPlayer(int playerId); 

	UFUNCTION(BlueprintCallable, Exec)
	void Spectate();

	UFUNCTION(BlueprintCallable, Exec)
	void SpectateNextPlayer();

	UFUNCTION(BlueprintCallable, Exec)
	void SpectatePreviousPlayer();


	UFUNCTION(BlueprintCallable, Exec)
	void sayControllerID();




protected:
	void OnPossess(APawn* aPawn) override;

	void OnUnPossess() override;


	void BeginPlay() override;

private:
	UFUNCTION()
	void reactOnPawnDeath();

	IOnlineSubsystem* _system;
	IOnlineFriendsPtr _friendsInterface;
	IOnlineSessionPtr _sessionInterface;

	ACWarlockChar* _currentlySpectated = nullptr;

};
