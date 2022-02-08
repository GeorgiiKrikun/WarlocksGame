// Fill out your copyright notice in the Description page of Project Settings.


#include "CMineActorServer.h"
#include "CMine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GoglikeLogging.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CWarlockChar.h"

// Sets default values
ACMineActorServer::ACMineActorServer() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!_collisionSpherePull) {
		_collisionSpherePull = CreateDefaultSubobject<USphereComponent>(TEXT("Sphereomponent"));
		this->SetRootComponent(_collisionSpherePull);
		_collisionSpherePull->SetCollisionResponseToAllChannels(ECR_Overlap);
		_collisionSpherePull->SetVisibility(true);
		_collisionSpherePull->bHiddenInGame = false;
	}

	if (!_collisionSphereSilence) {
		_collisionSphereSilence = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponentSilence"));
		/*this->SetRootComponent(_collisionSphereSilence);*/
		_collisionSphereSilence->SetCollisionResponseToAllChannels(ECR_Overlap);
		_collisionSphereSilence->SetVisibility(true);
		_collisionSphereSilence->bHiddenInGame = false;
	}

	if (!_movement)
	{
		// Use this component to drive this projectile's movement.
		_movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		_movement->SetUpdatedComponent(this->GetRootComponent());
		_movement->MaxSpeed = 300.0f;
		_movement->ProjectileGravityScale = 0.0f;
		_movement->SetIsReplicated(true);
	}

	SetReplicates(true);
	bAlwaysRelevant = true;
	bOnlyRelevantToOwner = false;
}

// Called when the game starts or when spawned
void ACMineActorServer::BeginPlay()
{
	Super::BeginPlay();
	_currentSuckTime = _suckTime;

}

// Called every frame
void ACMineActorServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority()) GL("Server");
	if (!HasAuthority()) return;
 
	if ((_currentSuckTime > 0.0f)) {

		_currentSuckTime -= DeltaTime;


		TArray<AActor*> overlappingActors;
		_collisionSpherePull->GetOverlappingActors(overlappingActors);

		for (AActor* actor : overlappingActors) {
			ACharacter* character = Cast<ACharacter>(actor);
			if (!character) continue;
			FVector direction = GetActorLocation() - actor->GetActorLocation();
			float distance = direction.Size();
			float forceFactor = (_collisionSpherePull->GetScaledSphereRadius() - distance) /(_collisionSpherePull->GetScaledSphereRadius());
			forceFactor = 0.75f + forceFactor / 2.0f;

			direction = direction.GetSafeNormal2D();
			direction = direction * _force;
			character->GetCharacterMovement()->AddForce(direction*forceFactor);
			GL("Force applied = %f, %f, %f", direction.X, direction.Y, forceFactor);
		}	

		overlappingActors.Empty();

		_collisionSpherePull->GetOverlappingActors(overlappingActors);

		for (AActor* actor : overlappingActors) {
			ACWarlockChar* character = Cast<ACWarlockChar>(actor);
			if (!character) continue;
			character->Silence(2.0f);
		}

		if (_currentSuckTime <= 0.0f) {
			Destroy();
		}
	}

	

}

UMovementComponent* ACMineActorServer::ProjectileMovement() const
{
	return _movement;
}

