// Fill out your copyright notice in the Description page of Project Settings.


#include "CMineActorServer.h"
#include "CMine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GoglikeLogging.h"
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

	SetReplicates(true);
	bAlwaysRelevant = true;
	bOnlyRelevantToOwner = false;
}

// Called when the game starts or when spawned
void ACMineActorServer::BeginPlay()
{
	Super::BeginPlay();
	//if (_collisionSphere) {
	//	_collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACMineActorServer::whenOverlapped);
	//	_collisionSphere->OnComponentEndOverlap.AddDynamic(this, &ACMineActorServer::whenNotOverlapped);
	//}

	if (HasAuthority()) GL("ServerBP");
	if (!HasAuthority()) GL("ClientBP");

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
		_collisionSphere->GetOverlappingActors(overlappingActors);

		for (AActor* actor : overlappingActors) {
			ACharacter* character = Cast<ACharacter>(actor);
			if (!character) continue;
			FVector direction = GetActorLocation() - actor->GetActorLocation();
			direction = direction.GetSafeNormal2D();
			direction = direction * _force;
			character->GetCharacterMovement()->AddForce(direction);
			GL("Force applied = %f, %f", direction.X, direction.Y);
		}	

		if (_currentSuckTime <= 0.0f) {
			if (!_skillThatSpawnedThatActor) return;
			_skillThatSpawnedThatActor->DestroyAllMines(_correspondingNumberOfThisActor);
		}
	}

	

}

void ACMineActorServer::whenOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


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

