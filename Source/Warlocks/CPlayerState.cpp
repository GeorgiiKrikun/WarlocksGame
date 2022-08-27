// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerState.h"
#include "GoglikeLogging.h"

void ACPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPlayerState, _isDead);
	DOREPLIFETIME(ACPlayerState, _coins);
	DOREPLIFETIME(ACPlayerState, _diamonds);
	DOREPLIFETIME(ACPlayerState, _damageDone);
	DOREPLIFETIME(ACPlayerState, _damageReceived);
	DOREPLIFETIME(ACPlayerState, _deaths);
	DOREPLIFETIME(ACPlayerState, _kills);
}

void ACPlayerState::setDead(bool dead) {
	GL("OLD DEAD IS %d, new Dead IS %d", _isDead, dead)
	if (dead && !_isDead) {
		GW("BROADCAST ON %d", GetNetMode());
		Del.Broadcast(GetPlayerId());
	}
	_isDead = dead;
}

int ACPlayerState::Coins() const
{
	return _coins;
}

void ACPlayerState::SetCoins(int val)
{
	_coins = val;
}

float ACPlayerState::DamageDone() const { return _damageDone; }

void ACPlayerState::SetDamageDone(float val) { _damageDone = val; }

float ACPlayerState::DamageReceived() const { return _damageReceived; }

void ACPlayerState::SetDamageReceived(float val) { _damageReceived = val; }

int ACPlayerState::Kills() const { return _kills; }

void ACPlayerState::SetKills(int val) { _kills = val; }

int ACPlayerState::Deaths() const { return _deaths; }

void ACPlayerState::SetDeaths(int val) { _deaths = val; }
