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
#include "Components/InputComponent.h"
#include "CSkillBase.h"
#include "CGameStateBase.h"
// Sets default values
ACWarlockChar::ACWarlockChar() : ACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_HealthPoints = 100.0f;
	_MaxHealthPoints = 100.0f;

	//_teleportAnimation = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TeleportAnimation"));
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
	//_teleportAnimation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	_sacrificeAnimation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TArray<UCSkillBase*> skillComponents;
	this->GetComponents<UCSkillBase>(skillComponents);
	
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

		AController* wc = EventInstigator;
		if (!wc) wc = _previousDamageInstigator;
		ACWarlocksGameMode* gameMode = Cast<ACWarlocksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		AController* thisController = GetController();
		if (!thisController) {
			GW("No controller while taking damage");
			return 0.0f;
		}
		ACPlayerState* playerState = thisController->GetPlayerState<ACPlayerState>();
		ACPlayerState* instigatorPlayerState = nullptr;
		if (wc) instigatorPlayerState = wc->GetPlayerState<ACPlayerState>();

		// coin awards stuff
		if (wc) {
			gameMode->addDamageStatisticsEntry(wc, Damage);
			_previousDamageInstigator = wc;
		}

		// player state change stuff
		if (_HealthPoints <= 0.0f) {
			if (playerState && !_isDead && gameMode->AreWeInMatch()) playerState->SetDeaths(playerState->Deaths() + 1);
			if (instigatorPlayerState && !_isDead && gameMode->AreWeInMatch()) instigatorPlayerState->SetKills(instigatorPlayerState->Kills() + 1);
			if (!_isDead) {
				FVector spawnLocation = this->GetActorLocation();
				onDeathDelegate.Broadcast();
				FRotator spawnRotation(0.0f, 0.0f, 0.0f);
				//ACDiamondActor* spawnedDiamond = Cast<ACDiamondActor>(GetWorld()->SpawnActor(_diamondClass, &spawnLocation, &spawnRotation));
			}
		}

		if (playerState && !_isDead && gameMode->AreWeInMatch()) playerState->SetDamageReceived(playerState->DamageReceived() + Damage);
		if (instigatorPlayerState && !_isDead && gameMode->AreWeInMatch()) instigatorPlayerState->SetDamageDone(instigatorPlayerState->DamageDone() + Damage);


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


	AController* wc = EventInstigator;
	if (!wc) wc = _previousDamageInstigator;
	ACWarlocksGameMode* gameMode = Cast<ACWarlocksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AController* thisController = GetController();
	if (!thisController) {
		GW("No controller while taking damage");
		return 0.0f;
	}
	ACPlayerState* playerState = thisController->GetPlayerState<ACPlayerState>();
	ACPlayerState* instigatorPlayerState = nullptr;
	if (wc) instigatorPlayerState = wc->GetPlayerState<ACPlayerState>();

	// coin awards stuff
	if (wc) {
		gameMode->addDamageStatisticsEntry(wc, Damage);
		_previousDamageInstigator = wc;
	}

	// player state change stuff
	if (_HealthPoints <= 0.0f) {
		if (playerState && !_isDead && gameMode->AreWeInMatch()) playerState->SetDeaths(playerState->Deaths() + 1);
		if (instigatorPlayerState && !_isDead && gameMode->AreWeInMatch()) instigatorPlayerState->SetKills(instigatorPlayerState->Kills() + 1);
		if (!_isDead) {
			FVector spawnLocation = this->GetActorLocation();
			onDeathDelegate.Broadcast();
			FRotator spawnRotation(0.0f, 0.0f, 0.0f);
			//ACDiamondActor* spawnedDiamond = Cast<ACDiamondActor>(GetWorld()->SpawnActor(_diamondClass, &spawnLocation, &spawnRotation));
		}
	}

	if (playerState && !_isDead && gameMode->AreWeInMatch()) playerState->SetDamageReceived(playerState->DamageReceived() + Damage);
	if (instigatorPlayerState && !_isDead && gameMode->AreWeInMatch()) instigatorPlayerState->SetDamageDone(instigatorPlayerState->DamageDone() + Damage);



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
		this->GetMesh()->SetSimulatePhysics(true);
		UCapsuleComponent* capsule = this->GetCapsuleComponent();
		capsule->SetCollisionResponseToChannels(_collisionDead);
		this->GetMovementComponent()->Deactivate();
	}
	else if (HasAuthority() && !_isDead)
	{
		GL("SET UNDEAD");
		_HealthPoints = MaxHealthPoints();
		this->GetMesh()->SetSimulatePhysics(false);
		this->GetMesh()->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		this->GetMesh()->SetRelativeRotation(_relativeMeshRotation);
		UCapsuleComponent* capsule = this->GetCapsuleComponent();
		capsule->SetCollisionResponseToChannels(_collisionDefault);
		this->GetMovementComponent()->Activate();

	}
}

void ACWarlockChar::OnRep_Dead()
{
	if (_isDead) {
		this->GetMesh()->SetSimulatePhysics(true);
		UCapsuleComponent* capsule = this->GetCapsuleComponent();
		this->GetMovementComponent()->Deactivate();
		capsule->SetCollisionResponseToChannels(_collisionDead);
	} else 
	{
		this->GetMesh()->SetSimulatePhysics(false);
		this->GetMesh()->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		this->GetMovementComponent()->Activate();
		this->GetMesh()->SetRelativeRotation(_relativeMeshRotation);
		UCapsuleComponent* capsule = this->GetCapsuleComponent();
		capsule->SetCollisionResponseToChannels(_collisionDefault);
	}
}	
// Called every frame
void ACWarlockChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ( _skillBeingCasted && (_skillBeingCasted->CurrentCastTime() <= 0.0f)) {
		_acceptsMovementInput = true;
		_skillBeingCasted = nullptr;
	}
}

void ACWarlockChar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACWarlockChar::Move_XAxis);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACWarlockChar::Move_YAxis);
}

void ACWarlockChar::Move_XAxis(float AxisValue)
{
	if (_acceptsMovementInput) AddMovementInput(FVector(AxisValue, 0.0f, 0.0f));
}

void ACWarlockChar::Move_YAxis(float AxisValue)
{
	if (_acceptsMovementInput) AddMovementInput(FVector(0.0f, AxisValue, 0.0f));
}

void ACWarlockChar::setCasting(UCSkillBase* skill)
{
	_skillBeingCasted = skill;
	_acceptsMovementInput = false;
}

UCSkillBase* ACWarlockChar::getSkillBeingCasted()
{
	return _skillBeingCasted;
}

//void ACWarlockChar::playTeleportAnimation_Implementation(float time)
//{
//	GL("playTeleportAnimation_Implementation");
//	_teleportAnimation->SetVisibility(true);
//	FTimerHandle handle;
//	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
//		_teleportAnimation->SetVisibility(false);
//		}, time, false);
//}

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



void ACWarlockChar::Silence(float duration)
{
	TArray<UCSkillBase*> skillComponents;
	this->GetComponents<UCSkillBase>(skillComponents);

	for (int i = 0; i < skillComponents.Num(); ++i) {
		float currentCooldown = skillComponents[i]->CurrentCooldown();
		if (currentCooldown < duration) {
			skillComponents[i]->SetCurrentCooldown(duration);
		}
	}
}

void  ACWarlockChar::stopMovementFor_Implementation(float seconds) 
{

	//_acceptsMovementInput = false;
	//FTimerHandle handle;
	//GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
	//		this->_acceptsMovementInput = true;
	//	}, seconds, false);
}

void ACWarlockChar::orientDirectionTowards_Implementation(FVector direction, float seconds) 
{
	FRotator rot = direction.ToOrientationRotator();
	rot.Pitch = 0.0f;
	rot.Roll = 0.0f;
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



void ACWarlockChar::die()
{
	SetDead(true);
}

void ACWarlockChar::setInvulnerable_Implementation(bool invulnerable)
{
	_invulnerable = invulnerable;

}

bool ACWarlockChar::GetInvulnerable()
{
	return _invulnerable;
}
