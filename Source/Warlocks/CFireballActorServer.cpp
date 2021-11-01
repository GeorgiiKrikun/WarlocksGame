// Fill out your copyright notice in the Description page of Project Settings.


#include "CFireballActorServer.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACFireballActorServer::ACFireballActorServer() : Super()
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

	if (!_movement)
	{
		// Use this component to drive this projectile's movement.
		_movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		_movement->SetUpdatedComponent(this->GetRootComponent());
		_movement->MaxSpeed = 3000.0f;
		_movement->ProjectileGravityScale = 0.0f;
	}	
}

// Called when the game starts or when spawned
void ACFireballActorServer::BeginPlay()
{
	Super::BeginPlay();
	if (_collisionSphere) {
		_collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACFireballActorServer::whenOverlapped);
		_collisionSphere->OnComponentEndOverlap.AddDynamic(this, &ACFireballActorServer::whenNotOverlapped);
	}
	
}

// Called every frame
void ACFireballActorServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACFireballActorServer::whenOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//this->Destroy();
	if (HasAuthority()) {
		TArray<AActor*> ingnoredActors;
		UGameplayStatics::ApplyRadialDamage(GetWorld(), _damage, this->GetActorLocation(), 200.0f, 0, ingnoredActors);
	}
	this->Destroy();

}

void ACFireballActorServer::whenNotOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));

}

