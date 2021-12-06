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

	_nPlayers++;
	ACPlayerState* state = NewPlayer->GetPlayerState<ACPlayerState>();
	if (state) GL("GOT NEW PLAYER AND HIS STATE %d ", GetNetMode());
	state->Del.AddDynamic(this, &ACWarlocksGameMode::ReactOnDeath);

}

void ACWarlocksGameMode::ReactOnDeath(int ID) {
	GL("Heh %d", ID);
	_nPlayers--;

	if (_nPlayers <= 1) {
		GW("This game has ended, moving to the next map");
		UWorld* World = GetWorld();
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ACWarlockMainPlayerController* PlayerController = Cast< ACWarlockMainPlayerController>(Iterator->Get());
			if (PlayerController) {
				ACWarlockChar* pawn = PlayerController->GetPawn<ACWarlockChar>();
				if (pawn) {
					PlayerController->SetPawn(nullptr);
					pawn->Destroy();
				}
				ACPlayerState* state = PlayerController->GetPlayerState<ACPlayerState>();
				state->setDead(false);

				RestartPlayer(PlayerController);
				PlayerController->callOnPossess();
				_nPlayers++;
			}
		}
	}



}
