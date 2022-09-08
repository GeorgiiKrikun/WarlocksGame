// Fill out your copyright notice in the Description page of Project Settings.


#include "CWarlockMainPlayerController.h"
#include "GoglikeLogging.h"
#include "CWarlockChar.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include <Kismet/GameplayStatics.h>
#include "CPlayerState.h"
#include <Containers/UnrealString.h>

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

void ACWarlockMainPlayerController::lookAtPlayer(int playerId)
{
	GL("Looking at player id %d", playerId)
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), playerId);
	if (controller) {GL("Controller ok"); }

	if (controller && controller->GetPawn()) {
		GL("Pawn ok");
		SetViewTarget(controller->GetPawn());
	}
}

void ACWarlockMainPlayerController::Spectate()
{
	auto myChar = Cast<ACWarlockChar>(this->GetPawn());
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACWarlockChar::StaticClass(), actors);
	for (auto actor : actors) {
		auto nextChar = Cast<ACWarlockChar>(actor);
		if (nextChar && (nextChar != myChar) && (!nextChar->Dead())) {
			SetViewTarget(nextChar);
			_currentlySpectated = nextChar;
			break;
		}
	}
}

void ACWarlockMainPlayerController::SpectateNextPlayer()
{
	if (!_currentlySpectated) {
		Spectate();
		return;
	}

	auto myChar = Cast<ACWarlockChar>(this->GetPawn());
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACWarlockChar::StaticClass(), actors);
	if (actors.Num() <= 1) return;

	int currentIndex = -1;

	for (int i = 0; i < actors.Num(); ++i) {
		if (actors[i] != _currentlySpectated) continue;
		currentIndex = i;
	}

	if (currentIndex > -1) {
		int i = (currentIndex + 1) % actors.Num();
		while (i != currentIndex) {
			auto nextChar = Cast<ACWarlockChar>(actors[i]);
			if (nextChar && (nextChar != myChar) && (!nextChar->Dead())) {
				SetViewTarget(nextChar);
				_currentlySpectated = nextChar;
				return;
			}
			i = (i + 1) % actors.Num();
		}
	}

	Spectate();


	
}

void ACWarlockMainPlayerController::SpectatePreviousPlayer()
{
	if (!_currentlySpectated) {
		Spectate();
		return;
	}

	auto myChar = Cast<ACWarlockChar>(this->GetPawn());
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACWarlockChar::StaticClass(), actors);
	if (actors.Num() <= 1) return;

	int currentIndex = -1;

	for (int i = 0; i < actors.Num(); ++i) {
		if (actors[i] != _currentlySpectated) continue;
		currentIndex = i;
	}

	if (currentIndex > -1) {
		int i = (actors.Num() + currentIndex - 1) % actors.Num();
		while (i != currentIndex) {
			auto nextChar = Cast<ACWarlockChar>(actors[i]);
			if (nextChar && (nextChar != myChar) && (!nextChar->Dead())) {
				SetViewTarget(nextChar);
				_currentlySpectated = nextChar;
				return;
			}
			i = (actors.Num() + i - 1) % actors.Num();
		}
	}

	Spectate();

}

void ACWarlockMainPlayerController::sayControllerID()
{
	
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
		character->TeleportTo(FVector(0, 0, -900), character->GetActorRotation());
	}
}
