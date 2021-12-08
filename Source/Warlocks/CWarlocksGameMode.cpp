// Fill out your copyright notice in the Description page of Project Settings.

#include "CWarlocksGameMode.h"
#include "CGameStateBase.h"
#include "CPlayerState.h"
#include "GoglikeLogging.h"
#include "CWarlockChar.h"
#include "CWarlockMainPlayerController.h"


void ACWarlocksGameMode::StartPlay() {
	Super::StartPlay();
}

void ACWarlocksGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (_currentLengthOfInterlude >= 0.0f && ((_currentLengthOfInterlude - DeltaSeconds) <= 0.0f)) {
		GW("Exiting Interlude");
		ReactOnExitInterlude();
	}

	_currentLengthOfInterlude -= DeltaSeconds;
}

void ACWarlocksGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACWarlocksGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	ACWarlockMainPlayerController* myController = Cast<ACWarlockMainPlayerController>(NewPlayer);
	if (!myController) {
		GE("Login failed, controller is empty");
	}

	myController->OnPawnDeath.AddDynamic(this, &ACWarlocksGameMode::ReactOnDeath);

}

void ACWarlocksGameMode::ReactOnDeath()
{

	if (_currentLengthOfInterlude <= 0.0f) { // we are in the battle
		ReactOnDeathBattle();
	}
	else { // interlude, immediate respawn
		ReactOnDeathInterlude();
	}

}

void ACWarlocksGameMode::ReactOnDeathBattle()
{
	if (_bRespawnGuard) return;
    int numberPlayersLeftAlive = CheckNumberOfPlayersAlive();
    
    GL("Number of players still alive = %d", numberPlayersLeftAlive);
    
	// respawn everyone
    if (numberPlayersLeftAlive > 1) return;
    _bRespawnGuard = true;
    GW("This game has ended");
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
    	ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
    	RespawnPlayer(PlayerController);
    }
    _bRespawnGuard = false;

	// begin interlude
	_currentLengthOfInterlude = _lengthOfInterlude;
}

void ACWarlocksGameMode::ReactOnDeathInterlude()
{
	if (_bRespawnGuard) return;
	_bRespawnGuard = true;

	GL("Reacting on death in interlude")
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		RespawnPlayer(PlayerController, false);
	}
	_bRespawnGuard = false;
}


void ACWarlocksGameMode::ReactOnExitInterlude()
{
	if (_bRespawnGuard) return;
	_bRespawnGuard = true;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		RespawnPlayer(PlayerController, true);
	}
	_bRespawnGuard = false;
}

int ACWarlocksGameMode::CheckNumberOfPlayersAlive()
{
	int numberPlayersLeftAlive = 0;
	UWorld* World = GetWorld();
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		if (PlayerController) {
			ACWarlockChar* pawn = PlayerController->GetPawn<ACWarlockChar>();
			if (pawn) numberPlayersLeftAlive++;
		}
	}
	return numberPlayersLeftAlive;
}

void ACWarlocksGameMode::RespawnPlayer(ACWarlockMainPlayerController* controller, bool respawnEvenIfAlive /*= true*/)
{
	if (!controller) return;
	ACWarlockChar* pawn = controller->GetPawn<ACWarlockChar>();
	if (pawn && respawnEvenIfAlive) {
		pawn->Destroy();
		RestartPlayer(controller);
	} else if (!pawn) {
		RestartPlayer(controller);
	}

}
