// Fill out your copyright notice in the Description page of Project Settings.

#include "CMine.h"
#include "CWarlockChar.h"
#include "CMineActorServer.h"
#include "Chaos/Map.h"
#include "GoglikeLogging.h"

UCMine::UCMine() : UCSkillBase(),
	_visibleOnThisInstance(false)
{

}


int UCMine::getRequiredInputType()
{
	return 1;
}

void UCMine::ServerSkillCast_Implementation(FVector location)
{
	if (_currentCooldown > 0.0f) return;
	_currentCooldown = _cooldown;

	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	FVector warlockLocation = warlock->GetActorLocation();
	FVector direction = (location - warlockLocation).GetSafeNormal2D();
	FVector spawnLocation = warlockLocation + 100.0f * direction;
	FRotator spawnRotation = direction.ToOrientationRotator();

	ACMineActorServer* spawnedMine = Cast<ACMineActorServer>(GetWorld()->SpawnActor(_mine, &spawnLocation, &spawnRotation));
	spawnedMine->SetSkillThatSpawnedThatActor(this);
}
