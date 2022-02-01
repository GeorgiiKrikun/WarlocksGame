// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARLOCKS_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	IOnlineSessionPtr _SessionInterface;
	
	TSharedPtr<FOnlineSessionSearch> _SessionSearch;

	virtual void Init() override;

	virtual void OnSessionCreateComplete(FName ServerName, bool Succeded);

	virtual void OnFindSessionComplete(bool Succeded);

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);	

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer(APlayerController* controller);

	APlayerController* _playerControllerThatUsedSearch;

		

	
};
