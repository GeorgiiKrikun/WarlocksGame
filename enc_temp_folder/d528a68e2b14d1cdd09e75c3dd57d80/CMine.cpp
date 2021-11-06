// Fill out your copyright notice in the Description page of Project Settings.

#include "CMine.h"
#include "CWarlockChar.h"
#include "CMineActorServer.h"
#include "Chaos/Map.h"

void UCMine::onBEGINCASTServer_Implementation(FVector location, FVector direction)
{
	SpawnMine(location, direction);
}

UCMine::UCMine() : UCSkillBase(),
	_currentActorSpawnedNumber(0),
	_visibleOnThisInstance(false)
{

}

void UCMine::SpawnMine_Implementation(FVector location, FVector direction)
{
	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	FVector warlockLocation = warlock->GetActorLocation();
	direction = (location - warlockLocation).GetSafeNormal2D();
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

	BeginAFTERCAST();
}

void UCMine::DestroyAllMines_Implementation(int32 num)
{
	ACMineActorServer** mine = _spawnedMines.Find(num);
	if (mine) {
		(*mine)->Destroy();
	}
}
