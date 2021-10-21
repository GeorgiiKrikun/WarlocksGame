// Fill out your copyright notice in the Description page of Project Settings.


#include "CWarlockChar.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACWarlockChar::ACWarlockChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_HP = 100;
}

void ACWarlockChar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void ACWarlockChar::BeginPlay()
{
	Super::BeginPlay();
	
}

float ACWarlockChar::InternalTakeRadialDamage(float Damage, struct FRadialDamageEvent const& RadialDamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority()) {
		Damage = Super::InternalTakeRadialDamage(Damage, RadialDamageEvent, EventInstigator, DamageCauser);
		_HP -= Damage;
		FVector origin = RadialDamageEvent.Origin;
		FVector actorLocation = GetActorLocation();
		FVector directionNormalized = origin - actorLocation;
		directionNormalized.Normalize();
		FVector finalImpulse = -directionNormalized * 100000.0f;
		GetCharacterMovement()->AddImpulse(finalImpulse, false);
		return Damage;
	}
	else {
		return 0;
	}

}

float ACWarlockChar::InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::InternalTakePointDamage(Damage, PointDamageEvent, EventInstigator, DamageCauser);
	_HP -= Damage;
	return Damage;
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

