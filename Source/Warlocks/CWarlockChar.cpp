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
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
// Sets default values
ACWarlockChar::ACWarlockChar() : ACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_HealthPoints = 100.0f;
	_MaxHealthPoints = 100.0f;

	_teleportAnimation = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TeleportAnimation"));
	_sacrificeAnimation = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SacrificeAnimation"));

	_invulnerable = false;


	//_teleportAnimation->SetAttachement(BombMesh);
	//PulseParticles->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

}

void ACWarlockChar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACWarlockChar, _HealthPoints);
	DOREPLIFETIME(ACWarlockChar, _MaxHealthPoints);
	DOREPLIFETIME(ACWarlockChar, _isDead);
	DOREPLIFETIME(ACWarlockChar, _invulnerable);
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

void ACWarlockChar::OnRep_Dead()
{
	UCapsuleComponent* capsule = this->GetCapsuleComponent();
	if (_isDead)  capsule->SetCollisionResponseToChannels(_collisionDead);
	else capsule->SetCollisionResponseToChannels(_collisionDefault);
}

float ACWarlockChar::InternalTakeRadialDamage(float Damage, struct FRadialDamageEvent const& RadialDamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority()) {
		if (_invulnerable) return 0.0f ;
		_HealthPoints = _HealthPoints - Damage;
		FVector origin = RadialDamageEvent.Origin;
		FVector actorLocation = GetActorLocation();
		FVector directionNormalized = origin - actorLocation;
		directionNormalized.Normalize();
		FVector finalImpulse = -directionNormalized * 100000.0f;
		GetCharacterMovement()->AddImpulse(finalImpulse, false);

		ACWarlockMainPlayerController* wc = Cast<ACWarlockMainPlayerController>(EventInstigator);
		ACWarlocksGameMode* gameMode = Cast<ACWarlocksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (wc) {
			gameMode->addDamageStatisticsEntry(wc, Damage);
			_previousDamageInstigator = wc;
		}
		else if (_previousDamageInstigator) {
			gameMode->addDamageStatisticsEntry(_previousDamageInstigator, Damage);
		}

		if (_HealthPoints <= 0.0f) {
			onDeathDelegate.Broadcast();
		}

		return Damage;


	}
	else {
		return 0;
	}

}

float ACWarlockChar::InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::InternalTakePointDamage(Damage, PointDamageEvent, EventInstigator, DamageCauser);
	_HealthPoints = _HealthPoints - Damage;

	ACWarlockMainPlayerController* wc = Cast<ACWarlockMainPlayerController>(EventInstigator);
	ACWarlocksGameMode* gameMode = Cast<ACWarlocksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (wc) {
		gameMode->addDamageStatisticsEntry(wc, Damage);
		_previousDamageInstigator = wc;
	}
	else if (_previousDamageInstigator) {
		gameMode->addDamageStatisticsEntry(_previousDamageInstigator, Damage);
	}

	if (_HealthPoints < 0.0f) {
		onDeathDelegate.Broadcast();
	}

	return Damage;
}

void ACWarlockChar::Restart()
{
	Super::Restart();
	GW("THIS FUCNTION SHOULD NOT BE USED, WDYD?");
	_previousDamageInstigator = nullptr;
	if (HasAuthority()) {
		GL("RESTART CHARACTER");
		SetDead(false);
		_HealthPoints = MaxHealthPoints();
	}
	if (!HasAuthority()) { // setup the player controller HUD again
		ACWarlockMainPlayerController* controller = GetController<ACWarlockMainPlayerController>();
		controller->callOnPawnRestartClient();
	}
}

void ACWarlockChar::SetDead_Implementation(bool val)
{
	_isDead = val;
	if (HasAuthority() && _isDead) {
		GL("SET DEAD");
		this->SetHidden(true);
		UCapsuleComponent* capsule = this->GetCapsuleComponent();
		capsule->SetCollisionResponseToChannels(_collisionDead);
	}
	else if (HasAuthority() && !_isDead)
	{
		GL("SET UNDEAD");
		_HealthPoints = MaxHealthPoints();
		this->SetHidden(false);
		UCapsuleComponent* capsule = this->GetCapsuleComponent();
		capsule->SetCollisionResponseToChannels(_collisionDefault);
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

void ACWarlockChar::setInvulnerable_Implementation(float duration)
{
	_invulnerable = true;

	GW("CHARACTER IS INVP");
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		this->_invulnerable = false;
		GW("!CHARACTER IS INVP");
	}, duration, false);
}
