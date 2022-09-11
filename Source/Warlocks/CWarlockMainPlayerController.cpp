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
	SpectateMyself();
	onInterludeBegin(text);
}

void ACWarlockMainPlayerController::onInterludeBegin_Implementation(const FString& text)
{

}

void ACWarlockMainPlayerController::callOnInterludeEnd_Implementation()
{
	SpectateMyself();
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
			SpectateCharacter(nextChar);
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
				SpectateCharacter(nextChar);
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
				SpectateCharacter(nextChar);
				return;
			}
			i = (actors.Num() + i - 1) % actors.Num();
		}
	}

	Spectate();

}

void ACWarlockMainPlayerController::SpectateMyself()
{
	SpectateCharacter(Cast<ACWarlockChar>(this->GetPawn()));
}

void ACWarlockMainPlayerController::SpectateCharacter(ACWarlockChar* characterToSpectate)
{
	if (!characterToSpectate) {
		GW("No character to spectate, aborting");
		return;
	}

	SetViewTarget(characterToSpectate);
	//GL("Character %s is now spectating character %s ", *this->GetPawn()->GetName(), *characterToSpectate->GetController()->GetPawn()->GetName());
	_currentlySpectated = characterToSpectate;
	//characterToSpectate->onDeathDelegate.RemoveDynamic(this, &ACWarlockMainPlayerController::reactOnSpectatedPawnDeath);
	//characterToSpectate->onDeathDelegate.AddDynamic(this, &ACWarlockMainPlayerController::reactOnSpectatedPawnDeath);
}

void ACWarlockMainPlayerController::SetNumberOfDiamonds(int N)
{
	ServerSetNumberOfDiamonds(N);
}

void ACWarlockMainPlayerController::ServerSetNumberOfDiamonds_Implementation(int N)
{
	GetPlayerState<ACPlayerState>()->SetDiamonds(N);
}

void ACWarlockMainPlayerController::SetNumberOfCoins(int N)
{
	ServerSetNumberOfCoins(N);
}

void ACWarlockMainPlayerController::ServerSetNumberOfCoins_Implementation(int N)
{
	GetPlayerState<ACPlayerState>()->SetCoins(N);
}

void ACWarlockMainPlayerController::sayControllerID()
{
	
}

void ACWarlockMainPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	ACWarlockChar* character = Cast<ACWarlockChar>(aPawn);
	if (!character) return;
	SpectateMyself();
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
	//Spectate();
	APawn* pawn = this->GetPawn();
	if (!pawn) return;
	if (HasAuthority()) {
		ACWarlockChar* character = Cast<ACWarlockChar>(pawn);
		character->SetDead(true);
		OnPawnDeath.Broadcast();
		//character->TeleportTo(FVector(0, 0, -900), character->GetActorRotation());
	}
}

void ACWarlockMainPlayerController::reactOnSpectatedPawnDeath()
{
//	GL(" Character %s is now dead, observed by %s spectating next character", *_currentlySpectated->GetController()->GetPawn()->GetName(), *this->GetPawn()->GetName());
//	//_currentlySpectated->onDeathDelegate.RemoveDynamic(this, &ACWarlockMainPlayerController::reactOnSpectatedPawnDeath);
//	SpectateNextPlayer();
}
