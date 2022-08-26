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

	if (_skillLevel > 1) {
		spawnedFireball->SetDamage(_damage*_sizeAndDamageUpgradeFactor);
		spawnedFireball->SetActorRelativeScale3D(FVector(_sizeAndDamageUpgradeFactor, _sizeAndDamageUpgradeFactor, _sizeAndDamageUpgradeFactor));
	}
	else {
		spawnedFireball->SetDamage(_damage);
	}

	if (_skillLevel > 2) spawnedFireball->SetSurviveTime(_flightTime * _flightTimeUpgradeFactor);
	else spawnedFireball->SetSurviveTime(_flightTime);

	if (_skillLevel > 3) projectileMovement->SetVelocityInLocalSpace(FVector(_velocity * _velocityUpgradeFactor, 0.0f, 0.0f));
	else projectileMovement->SetVelocityInLocalSpace(FVector(_velocity, 0.0f, 0.0f));



	spawnedFireball->SetSkillThatSpawnedThatActor(this);

}

void UCFireball::ServerSkillCast_Implementation(FVector location)
{
	Super::ServerSkillCast_Implementation(location);
	if (CurrentCooldown() > 0.0f) return;
	SetCurrentCooldown(_cooldown);

	startCastTime();

	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	warlock->orientDirectionTowards((location - warlock->GetActorLocation()), 1.0f);

}
