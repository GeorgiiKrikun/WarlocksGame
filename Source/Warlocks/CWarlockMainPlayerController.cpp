// Fill out your copyright notice in the Description page of Project Settings.


#include "CWarlockMainPlayerController.h"
#include "GoglikeLogging.h"
#include "CWarlockChar.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

void ACWarlockMainPlayerController::callOnPawnRestartClient_Implementation()
{
	GL("callOnPawnRestartClient_Implementation");
	OnPawnRestartClient();
}

void ACWarlockMainPlayerController::OnPawnRestartClient_Implementation()
{
	
}

void ACWarlockMainPlayerController::callOnInterludeBegin_Implementation(const FString& text)
{
	onInterludeBegin(text);
}

void ACWarlockMainPlayerController::onInterludeBegin_Implementation(const FString& text)
{

}

void ACWarlockMainPlayerController::callOnInterludeEnd_Implementation()
{
	onInterludeEnd();
}

void ACWarlockMainPlayerController::onInterludeEnd_Implementation()
{

}

bool ACWarlockMainPlayerController::connectToSteam()
{
	IOnlineSubsystem* system = IOnlineSubsystem::Get();
	if (system) {
		GW("Online system found, %s", *(system->GetSubsystemName().ToString()));
		_system = system;
		_friendsInterface = _system->GetFriendsInterface();
		_sessionInterface = _system->GetSessionInterface();

		return true;
	}
	else {
		GE("ONLINE SYSTEM ABSENT");
		return false;
	}
}

void ACWarlockMainPlayerController::createLobby(FName Name /*= TEXT("HEH")*/)
{
	FOnlineSessionSettings settings;
	_sessionInterface->CreateSession(0, Name, settings);
}


void ACWarlockMainPlayerController::scanSessions()
{
	int32 numSessions = _sessionInterface->GetNumSessions();
	GW("Number of sessions %d", numSessions);
}

void ACWarlockMainPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	ACWarlockChar* character = Cast<ACWarlockChar>(aPawn);
	if (!character) return;

	character->onDeathDelegate.AddDynamic(this, &ACWarlockMainPlayerController::reactOnPawnDeath);
}

void ACWarlockMainPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	if (HasAuthority()) {
		GL("OnUnPossess");
		OnPawnDeath.Broadcast();
	}
}

void ACWarlockMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACWarlockMainPlayerController::reactOnPawnDeath()
{
	GL("REACT ON PAWN DEATH");
	APawn* pawn = this->GetPawn();
	if (!pawn) return;
	if (HasAuthority()) {
		ACWarlockChar* character = Cast<ACWarlockChar>(pawn);
		character->SetDead(true);
		OnPawnDeath.Broadcast();
	}

	//UnPossess();
	//pawn->Destroy();
}
