// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerState.h"
#include "GoglikeLogging.h"
void ACPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPlayerState, _isDead);
}

void ACPlayerState::setDead(bool dead) {
	if (dead && !_isDead) {
		GW("BROADCAST ON %d", GetNetMode());
		Del.Broadcast(GetPlayerId());
	}
	_isDead = dead;

}