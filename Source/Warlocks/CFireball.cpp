// Fill out your copyright notice in the Description page of Project Settings.

#include "CFireball.h"
#include "CWarlockChar.h"
#include "CFireballActorServer.h"
#include "GoglikeLogging.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UCFireball::SpawnFireball_Implementation(FVector location)
{
	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	FVector warlockLocation = warlock->GetActorLocation();

	FVector direction = (location - warlockLocation).GetSafeNormal2D();
	FVector spawnLocation = warlockLocation + 100.0f * direction;
	FRotator spawnRotation = direction.ToOrientationRotator();
		
	if (warlock->HasAuthority()) {
		ACFireballActorServer* spawnedFireball = GetWorld()->SpawnActor<ACFireballActorServer>(spawnLocation,spawnRotation);
		spawnedFireball->ProjectileMovement()->SetVelocityInLocalSpace(FVector(1000.0f,0.0f,0.0f) );
		_spawnedFireballs.Add(_currentActorSpawnedNumber, spawnedFireball);
		spawnedFireball->SetSkillThatSpawnedThisActor(this);
		spawnedFireball->SetCorrespondingNumberOfThisActor(_currentActorSpawnedNumber);
		_currentActorSpawnedNumber++;
	}
	else {
		if (!_fireball) {
			return;
		}
		AActor* spawnActor = GetWorld()->SpawnActor(_fireball, &spawnLocation, &spawnRotation);
		ACFireballActorServer* spawnedFireball = Cast<ACFireballActorServer>(spawnActor);
		spawnedFireball->ProjectileMovement()->SetVelocityInLocalSpace(FVector(1000.0f, 0.0f, 0.0f));
		_spawnedFireballs.Add(_currentActorSpawnedNumber, spawnedFireball);
		spawnedFireball->SetSkillThatSpawnedThisActor(this);
		spawnedFireball->SetCorrespondingNumberOfThisActor(_currentActorSpawnedNumber);
		_currentActorSpawnedNumber++;
	}

}

void UCFireball::DestroyAllMines_Implementation(int32 num)
{
	ACFireballActorServer** fireball = _spawnedFireballs.Find(num);
	if (fireball && (*fireball)) {
		(*fireball)->Destroy();
	}
}

int UCFireball::getRequiredInputType()
{
	return 1;
}

void UCFireball::ServerSkillCast_Implementation(FVector location)
{
	if (_currentCooldown > 0.0f) return;
	_currentCooldown = _cooldown;
	GL("Fireball cooldowns %f %f", _currentCooldown, _cooldown);

	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	warlock->playFireballAnimation();

	warlock->stopMovementFor(_castDelay);
	warlock->orientDirectionTowards(location - warlock->GetActorLocation(), 1.0f);

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this, location]() {
		SpawnFireball(location);
	}, _castDelay, false);
}
