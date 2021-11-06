// Fill out your copyright notice in the Description page of Project Settings.


#include "CMineActorServer.h"
#include "CMine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
ACMineActorServer::ACMineActorServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!_collisionSphere) {
		_collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphereomponent"));
		this->SetRootComponent(_collisionSphere);
		_collisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
		_collisionSphere->SetVisibility(true);
		_collisionSphere->bHiddenInGame = false;
	}
}

// Called when the game starts or when spawned
void ACMineActorServer::BeginPlay()
{
	Super::BeginPlay();
	if (_collisionSphere) {
		_collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACMineActorServer::whenOverlapped);
		_collisionSphere->OnComponentEndOverlap.AddDynamic(this, &ACMineActorServer::whenNotOverlapped);
	}
}

// Called every frame
void ACMineActorServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACMineActorServer::whenOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		TArray<AActor*> ingnoredActors;
		UGameplayStatics::ApplyRadialDamage(GetWorld(), _damage, this->GetActorLocation(), 200.0f, 0, ingnoredActors);
		if (!_skillThatSpawnedThatActor) return;
		_skillThatSpawnedThatActor->DestroyAllMines(_correspondingNumberOfThisActor);
	}

}

void ACMineActorServer::whenNotOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACMineActorServer::SetSkillThatSpawnedThisActor(UCMine* mine)
{
	_skillThatSpawnedThatActor = mine;
}

void ACMineActorServer::SetCorrespondingNumberOfThisActor(int32 num)
{
	_correspondingNumberOfThisActor = num;
}

