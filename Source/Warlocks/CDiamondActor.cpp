// Fill out your copyright notice in the Description page of Project Settings.


#include "CDiamondActor.h"
#include "CWarlockChar.h"
#include "CPlayerState.h"
#include "CWarlocksGameMode.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ACDiamondActor::ACDiamondActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetReplicates(true);
	bReplicates = true;
	bAlwaysRelevant = true;
	bOnlyRelevantToOwner = false;
	//SetReplicatingMovement(true);

	if (!_collisionSphere) {
		_collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphereomponent"));
		this->SetRootComponent(_collisionSphere);
		_collisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
		_collisionSphere->SetVisibility(true);
		_collisionSphere->bHiddenInGame = false;
	}

}

// Called when the game starts or when spawned
void ACDiamondActor::BeginPlay()
{
	Super::BeginPlay();
	if (_collisionSphere) {
		_collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACDiamondActor::whenOverlapped);
	}
}

void ACDiamondActor::whenOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACWarlockChar* wc = Cast<ACWarlockChar>(otherActor);
	if (wc && !wc->Dead() && HasAuthority() && wc->GetController() && wc->GetController()->GetPlayerState<ACPlayerState>() && 
		Cast<ACWarlocksGameMode>(UGameplayStatics::GetGameMode(GetWorld())) && 
		Cast<ACWarlocksGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AreWeInMatch()) {
		ACPlayerState* state = wc->GetController()->GetPlayerState<ACPlayerState>();
		state->SetDiamonds(state->Diamonds() + 1);
		Destroy();
	}
}

// Called every frame
void ACDiamondActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

