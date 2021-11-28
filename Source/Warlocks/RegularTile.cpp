// Fill out your copyright notice in the Description page of Project Settings.


#include "RegularTile.h"
#include "Components/StaticMeshComponent.h" 
#include "Components/BoxComponent.h" 
#include "CWarlockChar.h"
#include "GoglikeLogging.h"

// Sets default values
ARegularTile::ARegularTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));


	if (!_collisionBox) {
		_collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
		_collisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
		_collisionBox->SetVisibility(true);
		_collisionBox->bHiddenInGame = false;
	}

}

// Called when the game starts or when spawned
void ARegularTile::BeginPlay()
{
	Super::BeginPlay();
	_collisionBox->SetWorldTransform(this->GetActorTransform());

}

// Called every frame
void ARegularTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;
	TArray<AActor*> overlappingActors;
	_collisionBox->GetOverlappingActors(overlappingActors);

	for (int i = 0; i < overlappingActors.Num(); ++i) {
		ACWarlockChar* warlock = dynamic_cast<ACWarlockChar*>(overlappingActors[i]);
		if (!warlock) continue;
		warlock->TakeDamage(10.0f * DeltaTime, FDamageEvent(), nullptr, nullptr);
		GL("Heh");
	}


}