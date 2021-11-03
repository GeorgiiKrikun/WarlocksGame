// Fill out your copyright notice in the Description page of Project Settings.

#include "CMine.h"
#include "CWarlockChar.h"
#include "CMineActorServer.h"

void UCMine::onBEGINCASTServer_Implementation(FVector location, FVector direction)
{
	SpawnMine(location, direction);
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
		ACMineActorServer* _spawnedMine = GetWorld()->SpawnActor<ACMineActorServer>(spawnLocation, spawnRotation);
	} else {
		if (!_mine) {
			return;
		}
		ACMineActorServer* _spawnedMine = GetWorld()->SpawnActor(_mine, &spawnLocation, &spawnRotation);
	}

	BEGINCASTClient(location, direction);
}
