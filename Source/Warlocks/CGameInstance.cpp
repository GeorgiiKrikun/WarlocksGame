// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GoglikeLogging.h"
#include "OnlineSessionSettings.h"

void UCGameInstance::Init()
{
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get("Steam")) {
		GL("SUBSYSTEM = %s", *SubSystem->GetOnlineServiceName().ToString());
		_SessionInterface = SubSystem->GetSessionInterface();
		if (_SessionInterface.IsValid()) {
			_SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnSessionCreateComplete);
			_SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionComplete);
			_SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnJoinSessionComplete);
		}
	}
}

void UCGameInstance::OnSessionCreateComplete(FName ServerName, bool Succeded)
{
	GL("Session create complete %d", Succeded);
	if (Succeded) {
		bool travel =  GetWorld()->ServerTravel("/Game/Maps/ServerArena?listen");
		if (travel) GL("Travel returned success");
		if (!travel) GL("Travel returned false");
	}
}

void UCGameInstance::OnFindSessionComplete(bool Succeded)
{
	GL("Session find complete, succesful = %d, number = %d", Succeded, _SessionSearch->SearchResults.Num());
	TArray<FOnlineSessionSearchResult> searchResults = _SessionSearch->SearchResults;
	if (Succeded && searchResults.Num()) {
		GL("Joining found server");
		_SessionInterface->JoinSession(0, FName(TEXT("Session")), searchResults[0]);
	}
	
	
}

void UCGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GL("Join Session Complete, joining game");	
	if (_playerControllerThatUsedSearch) {	
		GL("Player controller exits");	

		FString JoinAdress = "";
		_SessionInterface->GetResolvedConnectString(SessionName, JoinAdress);
		GL("FOUND JOIN ADRESS = %s", *JoinAdress )
		if (JoinAdress != "") {
			_playerControllerThatUsedSearch->ClientTravel(JoinAdress, ETravelType::TRAVEL_Absolute);
		}

	}


}

void UCGameInstance::CreateServer()
{
	GL("CReating session");
	FOnlineSessionSettings settings;

	settings.bAllowJoinInProgress = true;
	settings.bIsDedicated = false;
	settings.bIsLANMatch = false;
	settings.bShouldAdvertise = true;
	settings.bUsesPresence = true;
	settings.NumPublicConnections = 5;

	_SessionInterface->CreateSession(0, FName(TEXT("Session")), settings);
	GL("CReating session2");


}

void UCGameInstance::JoinServer(APlayerController* controller)
{
	_playerControllerThatUsedSearch = controller;

	_SessionSearch = MakeShareable(new FOnlineSessionSearch());
	_SessionSearch->bIsLanQuery = false;
	_SessionSearch->MaxSearchResults = 10000;
	_SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	_SessionInterface->FindSessions(0, _SessionSearch.ToSharedRef());

}
