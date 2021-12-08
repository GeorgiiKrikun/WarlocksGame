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
	if (_bRespawnGuard) return;
	UWorld* World = GetWorld();
	int numberPlayersLeftAlive = 0;
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		if (PlayerController) {
			ACWarlockChar* pawn = PlayerController->GetPawn<ACWarlockChar>();
			if (pawn) numberPlayersLeftAlive++;

			//ACPlayerState* state = PlayerController->GetPlayerState<ACPlayerState>();
			//state->setDead(false);

			//RestartPlayer(PlayerController);
			//PlayerController->callOnPawnRestartClient();
			//_nPlayers++;
		}
	}

	GL("Number of players still alive = %d", numberPlayersLeftAlive);

	if (numberPlayersLeftAlive > 1) return;
	_bRespawnGuard = true;
	GW("This game has ended");
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACWarlockMainPlayerController* PlayerController = Cast<ACWarlockMainPlayerController>(Iterator->Get());
		ACWarlockChar* pawn = PlayerController->GetPawn<ACWarlockChar>();
		if (pawn) pawn->Destroy();
		if (PlayerController) {
			RestartPlayer(PlayerController);
		}
	}
	_bRespawnGuard = false;
}
