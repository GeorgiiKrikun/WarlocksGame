// Fill out your copyright notice in the Description page of Project Settings.

#include "CFireball.h"
#include "CWarlockChar.h"
#include "CFireballActorServer.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UCFireball::onBEGINCASTServer_Implementation(FVector location, FVector direction)
{
	SpawnFireball(location, direction);
}

void UCFireball::SpawnFireball_Implementation(FVector location, FVector direction)
{
	auto warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	FVector warlockLocation = warlock->GetActorLocation();
	direction = (location - warlockLocation).GetSafeNormal2D();
	FVector spawnLocation = warlockLocation + 100.0f * direction;
	FRotator spawnRotation = direction.ToOrientationRotator();
		
	if (warlock->HasAuthority()) {
		ACFireballActorServer* spawnedFireball = GetWorld()->SpawnActor<ACFireballActorServer>(spawnLocation,spawnRotation);
		spawnedFireball->ProjectileMovement()->SetVelocityInLocalSpace(FVector(1000.0f,0.0f,0.0f) );
	}
	else {
		if (!_fireball) {
			return;
		}
		AActor* spawnActor = GetWorld()->SpawnActor(_fireball, &spawnLocation, &spawnRotation);
		ACFireballActorServer* spawnedFireball = Cast<ACFireballActorServer>(spawnActor);
		spawnedFireball->ProjectileMovement()->SetVelocityInLocalSpace(FVector(1000.0f, 0.0f, 0.0f));
	}

	BEGINCASTClient(location, direction);
}
