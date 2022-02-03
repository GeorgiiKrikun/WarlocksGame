// Fill out your copyright notice in the Description page of Project Settings.

#include "CFireball.h"
#include "CWarlockChar.h"
#include "CFireballActorServer.h"
#include "GoglikeLogging.h"
#include "GameFramework/ProjectileMovementComponent.h"

int UCFireball::getRequiredInputType()
{
	return 1;
}

void UCFireball::ServerAfterSkillCasted_Implementation(FVector location)
{
	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	FVector warlockLocation = warlock->GetActorLocation();
	FVector direction = (location - warlockLocation).GetSafeNormal2D();
	FVector spawnLocation = warlockLocation + 100.0f * direction;
	FRotator spawnRotation = direction.ToOrientationRotator();

	ACFireballActorServer* spawnedFireball = Cast<ACFireballActorServer>(GetWorld()->SpawnActor(_fireball, &spawnLocation, &spawnRotation));
	auto projectileMovement = Cast<UProjectileMovementComponent>(spawnedFireball->ProjectileMovement());
	projectileMovement->SetVelocityInLocalSpace(FVector(1000.0f, 0.0f, 0.0f));
	spawnedFireball->SetSkillThatSpawnedThatActor(this);

}

void UCFireball::ServerSkillCast_Implementation(FVector location)
{
	Super::ServerSkillCast_Implementation(location);
	if (_currentCooldown > 0.0f) return;
	_currentCooldown = _cooldown;

	startCastTime();

	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	warlock->orientDirectionTowards((location - warlock->GetActorLocation()), 1.0f);
	warlock->playFireballAnimation();



}
