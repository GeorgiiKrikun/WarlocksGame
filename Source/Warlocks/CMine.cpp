// Fill out your copyright notice in the Description page of Project Settings.

#include "CMine.h"
#include "CWarlockChar.h"
#include "CMineActorServer.h"
#include "Chaos/Map.h"
#include "GoglikeLogging.h"
#include "GameFramework/ProjectileMovementComponent.h"

UCMine::UCMine() : UCSkillBase(),
	_visibleOnThisInstance(false)
{

}


int UCMine::getRequiredInputType()
{
	return 1;
}

void UCMine::ServerAfterSkillCasted_Implementation(FVector location)
{
	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	FVector warlockLocation = warlock->GetActorLocation();
	FVector direction = (location - warlockLocation).GetSafeNormal2D();
	FVector spawnLocation = warlockLocation + 350.0f * direction;
	FRotator spawnRotation = direction.ToOrientationRotator();

	ACMineActorServer* spawnedMine = Cast<ACMineActorServer>(GetWorld()->SpawnActor(_mine, &spawnLocation, &spawnRotation));
	auto projectileMovement = Cast<UProjectileMovementComponent>(spawnedMine->ProjectileMovement());
	projectileMovement->SetVelocityInLocalSpace(FVector(100.0f, 0.0f, 0.0f));
	spawnedMine->SetSkillThatSpawnedThatActor(this);
}

void UCMine::ServerSkillCast_Implementation(FVector location)
{
	Super::ServerSkillCast_Implementation(location);

	if (CurrentCooldown() > 0.0f) return;
	SetCurrentCooldown(_cooldown);
	startCastTime();

	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	warlock->orientDirectionTowards((location - warlock->GetActorLocation()), 1.0f);


}
