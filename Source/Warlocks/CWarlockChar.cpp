// Fill out your copyright notice in the Description page of Project Settings.


#include "CWarlockChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GoglikeLogging.h"
#include "Net/UnrealNetwork.h"
#include "CWarlocksGameMode.h"
#include "GameFramework/GameModeBase.h" 
#include "CPlayerState.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Pawn.h"
#include "CWarlockMainPlayerController.h"
// Sets default values
ACWarlockChar::ACWarlockChar() : ACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_HealthPoints = 100.0f;
	_MaxHealthPoints = 100.0f;

	_teleportAnimation = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TeleportAnimation"));
	_sacrificeAnimation = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SacrificeAnimation"));


	//_teleportAnimation->SetAttachement(BombMesh);
	//PulseParticles->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

}

void ACWarlockChar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACWarlockChar, _HealthPoints);
	DOREPLIFETIME(ACWarlockChar, _MaxHealthPoints);
}

float ACWarlockChar::HealthPoints() const
{
	return _HealthPoints;
}

float ACWarlockChar::MaxHealthPoints() const
{
	return _MaxHealthPoints;
}

// Called when the game starts or when spawned
void ACWarlockChar::BeginPlay()
{
	Super::BeginPlay();
	_teleportAnimation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	_sacrificeAnimation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
}

float ACWarlockChar::InternalTakeRadialDamage(float Damage, struct FRadialDamageEvent const& RadialDamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority()) {
		Damage = Super::InternalTakeRadialDamage(Damage, RadialDamageEvent, EventInstigator, DamageCauser);
		GL("DAMAGE TAKEN %f", Damage);
		_HealthPoints = _HealthPoints - Damage;
		FVector origin = RadialDamageEvent.Origin;
		FVector actorLocation = GetActorLocation();
		FVector directionNormalized = origin - actorLocation;
		directionNormalized.Normalize();
		FVector finalImpulse = -directionNormalized * 100000.0f;
		GetCharacterMovement()->AddImpulse(finalImpulse, false);
		return Damage;
		if (_HealthPoints < 0.0f) {
			onDeathDelegate.Broadcast();
		}

	}
	else {
		return 0;
	}

}

float ACWarlockChar::InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::InternalTakePointDamage(Damage, PointDamageEvent, EventInstigator, DamageCauser);
	_HealthPoints = _HealthPoints - Damage;
	GL("TOOK POINT DAMAGE");

	if (_HealthPoints < 0.0f) {
		onDeathDelegate.Broadcast();
	}

	return Damage;
}

float ACWarlockChar::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	_HealthPoints = _HealthPoints - Damage;
	
	if (_HealthPoints < 0.0f) {
		onDeathDelegate.Broadcast();
	}

	return Damage;
}

void ACWarlockChar::Restart()
{
	Super::Restart();
	if (!HasAuthority()) { // setup the player controller HUD again
		ACWarlockMainPlayerController* controller = GetController<ACWarlockMainPlayerController>();
		controller->callOnPawnRestartClient();
	}
}

// Called every frame
void ACWarlockChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACWarlockChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void ACWarlockChar::playTeleportAnimation_Implementation(float time)
{
	GL("playTeleportAnimation_Implementation");
	_teleportAnimation->SetVisibility(true);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		_teleportAnimation->SetVisibility(false);
		}, time, false);

}

void ACWarlockChar::startPlaySacrificeAnimation_Implementation()
{
	playSacrificeAnimation();
}

void ACWarlockChar::playSacrificeAnimation_Implementation()
{
	if (HasAuthority()) return;
	ClientImplementationOfSacrificeAnimation();
}

void ACWarlockChar::ClientImplementationOfSacrificeAnimation_Implementation()
{

}



void  ACWarlockChar::stopMovementFor_Implementation(float seconds) 
{
	AController* controller = GetController();
	if (!controller) {
		GW("Could not fild controller, movement input failed");
	}

	controller->SetIgnoreMoveInput(true);

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [controller]() {
			controller->SetIgnoreMoveInput(false);
		}, seconds, false);
}

void ACWarlockChar::orientDirectionTowards_Implementation(FVector direction, float seconds) 
{
	FRotator rot = direction.ToOrientationRotator();
	this->SetActorRotation(rot, ETeleportType::None);
	GL("Rotator = %s", *rot.ToString());

}

void ACWarlockChar::playFireballAnimation_Implementation()
{
	if (HasAuthority()) return;
	ClientImplementationOfFireballAnimation();
}

void ACWarlockChar::ClientImplementationOfFireballAnimation_Implementation()
{

}

void ACWarlockChar::death_Implementation() {


}

