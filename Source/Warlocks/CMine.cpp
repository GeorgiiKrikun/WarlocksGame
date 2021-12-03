// Fill out your copyright notice in the Description page of Project Settings.

#include "CMine.h"
#include "CWarlockChar.h"
#include "CMineActorServer.h"
#include "Chaos/Map.h"
#include "GoglikeLogging.h"

UCMine::UCMine() : UCSkillBase(),
	_currentActorSpawnedNumber(0),
	_visibleOnThisInstance(false)
{

}


void UCMine::SpawnMine_Implementation(FVector location)
{
	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	FVector warlockLocation = warlock->GetActorLocation();
	FVector direction = (location - warlockLocation).GetSafeNormal2D();
	FVector spawnLocation = warlockLocation + 100.0f * direction;
	FRotator spawnRotation = direction.ToOrientationRotator();

	if (warlock->HasAuthority()) {
		ACMineActorServer* spawnedMine = GetWorld()->SpawnActor<ACMineActorServer>(spawnLocation, spawnRotation);
		_spawnedMines.Add(_currentActorSpawnedNumber, spawnedMine);
		spawnedMine->SetSkillThatSpawnedThisActor(this);
		spawnedMine->SetCorrespondingNumberOfThisActor(_currentActorSpawnedNumber);
		_currentActorSpawnedNumber++;

	} else {
		if (!_mine) {
			_currentActorSpawnedNumber++;
			return;
		}
		ACMineActorServer* spawnedMine = Cast<ACMineActorServer>(GetWorld()->SpawnActor(_mine, &spawnLocation, &spawnRotation));
		_spawnedMines.Add(_currentActorSpawnedNumber, spawnedMine);
		spawnedMine->SetSkillThatSpawnedThisActor(this);
		spawnedMine->SetCorrespondingNumberOfThisActor(_currentActorSpawnedNumber);
		_currentActorSpawnedNumber++;
		if (_visibleOnThisInstance) {
			spawnedMine->_visibleOnThisInstance = true;
		}

		spawnedMine->armVisibilityTimer();
	}

}

void UCMine::DestroyAllMines_Implementation(int32 num)
{
	ACMineActorServer** mine = _spawnedMines.Find(num);
	if (mine && (*mine)) {
		(*mine)->Destroy();
	}
}

int UCMine::getRequiredInputType()
{
	return 1;
}

void UCMine::ServerSkillCast_Implementation(FVector location)
{
	if (_currentCooldown > 0.0f) return;
	_currentCooldown = _cooldown;
	GL("Mine cooldowns %f %f", _currentCooldown, _cooldown);


	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	FVector warlockLocation = warlock->GetActorLocation();
	FVector direction = (location - warlockLocation).GetSafeNormal2D();
	FVector spawnLocation = warlockLocation + 100.0f * direction;
	FRotator spawnRotation = direction.ToOrientationRotator();

	ACMineActorServer* spawnedMine = Cast<ACMineActorServer>(GetWorld()->SpawnActor(_mine, &spawnLocation, &spawnRotation));
	_spawnedMines.Add(_currentActorSpawnedNumber, spawnedMine);
	spawnedMine->SetSkillThatSpawnedThisActor(this);
	spawnedMine->SetCorrespondingNumberOfThisActor(_currentActorSpawnedNumber);
	_currentActorSpawnedNumber++;

	//SpawnMine(location);
}
