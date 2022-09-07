// Fill out your copyright notice in the Description page of Project Settings.

#include "CFireball.h"
#include "CWarlockChar.h"
#include "CFireballActorServer.h"
#include "GoglikeLogging.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UCFireball::ServerCastDiamondUpgrade_Implementation(FVector location)
{
	Super::ServerCastDiamondUpgrade_Implementation(location);
	if (_currentCooldownDiamond <= 0.0f) return;
	_currentCooldownDiamond = -1.0f;

	startCastTime();

//	auto warlock = Cast<ACWarlockChar>(GetOwner());
//	if (!warlock) return;
//	warlock->orientDirectionTowards((location - warlock->GetActorLocation()), 1.0f);
}

void UCFireball::ServerAfterCastDiamondUpgrade_Implementation(FVector location)
{
	Super::ServerAfterCastDiamondUpgrade_Implementation(location);
	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	if (!_lastSpawnedFireball) return;


	FVector spawnLocation = _lastSpawnedFireball->GetActorLocation();
	FRotator spawnRotation = _lastSpawnedFireball->GetActorRotation();
	FVector direction = spawnRotation.Vector().GetUnsafeNormal();
	FRotator rightRotator(0.0f, 90.0f, 0.0f), leftRotator(0.0f, -90.0f, 0.0f);
	FVector  rightDirection = rightRotator.RotateVector(direction), leftDirection = leftRotator.RotateVector(direction);
	FVector spawnRightLocation = spawnLocation + rightDirection * 100.0f;
	FVector spawnLeftLocation = spawnLocation + leftDirection * 100.0f;
	FRotator spawnRightRotation = spawnRotation + rightRotator, spawnLeftRotation = spawnRotation + leftRotator;
		
	_lastSpawnedFireball->Destroy();
	_lastSpawnedFireball = nullptr;

	auto rightFireball = Cast<ACFireballActorServer>(GetWorld()->SpawnActor(_fireball, &spawnRightLocation, &spawnRightRotation));
	auto leftFireball = Cast<ACFireballActorServer>(GetWorld()->SpawnActor(_fireball, &spawnLeftLocation, &spawnLeftRotation));

	for (auto spawnedFireball : { rightFireball,leftFireball }) {
		auto projectileMovement = Cast<UProjectileMovementComponent>(spawnedFireball->ProjectileMovement());

		if (_skillLevel > 1) {
			spawnedFireball->SetDamage(_damage * _sizeAndDamageUpgradeFactor);
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


}

void UCFireball::onLevelUp()
{
	if (_skillLevel <= 2) _cooldownDiamond = _flightTime;
	else _cooldownDiamond = _flightTime * _flightTimeUpgradeFactor;
}

int UCFireball::getRequiredInputType()
{
	return 1;
}

void UCFireball::ServerAfterSkillCasted_Implementation(FVector location)
{
	Super::ServerAfterSkillCasted_Implementation(location);

	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	FVector warlockLocation = warlock->GetActorLocation();
	FVector direction = (location - warlockLocation).GetSafeNormal2D();
	FVector spawnLocation = warlockLocation + 100.0f * direction;
	FRotator spawnRotation = direction.ToOrientationRotator();

	_lastSpawnedFireball = Cast<ACFireballActorServer>(GetWorld()->SpawnActor(_fireball, &spawnLocation, &spawnRotation));
	auto spawnedFireball = _lastSpawnedFireball;
	if ( spawnedFireball) {
		auto projectileMovement = Cast<UProjectileMovementComponent>(_lastSpawnedFireball->ProjectileMovement());

		if (_skillLevel > 1) {
			spawnedFireball->SetDamage(_damage * _sizeAndDamageUpgradeFactor);
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
