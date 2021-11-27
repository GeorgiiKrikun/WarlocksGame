// Fill out your copyright notice in the Description page of Project Settings.


#include "RegularTile.h"
#include "Components/StaticMeshComponent.h" 


// Sets default values
ARegularTile::ARegularTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void ARegularTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARegularTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

