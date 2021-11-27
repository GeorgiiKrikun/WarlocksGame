// Fill out your copyright notice in the Description page of Project Settings.


#include "CTileSpawner.h"

// Sets default values
ACTileSpawner::ACTileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_horizontalSize = 0;
	_verticalSize = 0;
	_tilesNumberHorizontal = 0;
	_tilesNumberVertical = 0;
	_zOffset = 0;
}

// Called when the game starts or when spawned
void ACTileSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority()) return;
	FVector location = GetActorLocation();
	float horizontalOffset = location.X - _tilesNumberHorizontal * _horizontalSize/2.0f;
	float verticalOffset = location.Y - _tilesNumberVertical * _verticalSize / 2.0f;

	for (int h = 0; h < _tilesNumberHorizontal; ++h) {
		for (int v = 0; v < _tilesNumberVertical; ++v) {
			float x = horizontalOffset + h * _horizontalSize;
			float y = verticalOffset + v * _verticalSize;
			float z = location.Z - _zOffset;
			FVector spawnLocation(x,y,z);
			FRotator spawnRotation;
			AActor* spawnActor = GetWorld()->SpawnActor(_actorToSpawn, &spawnLocation, &spawnRotation);
		}
	}
}

// Called every frame
void ACTileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

