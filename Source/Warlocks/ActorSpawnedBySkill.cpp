// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSpawnedBySkill.h"
#include "CSkillBase.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CWarlockChar.h"

// Sets default values
AActorSpawnedBySkill::AActorSpawnedBySkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorSpawnedBySkill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorSpawnedBySkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UMovementComponent* AActorSpawnedBySkill::ProjectileMovement() const
{
	return nullptr;
}

bool AActorSpawnedBySkill::sendBackToSender(UCSkillBase* newSkillSender/*=nullptr*/)
{
	// try to send skill back if has projectile movement

	UCSkillBase* oldSkillSender = SkillThatSpawnedThatActor();
	if (!oldSkillSender) return false;

	SetSkillThatSpawnedThatActor(newSkillSender);


	UProjectileMovementComponent* projectileMovement = Cast< UProjectileMovementComponent> (ProjectileMovement());
	if ( (!projectileMovement) || (!oldSkillSender->GetOwner()) ) return false;
	
	projectileMovement->SetVelocityInLocalSpace(FVector(.0f, 0.0f, 0.0f));
	// find location of owner
	ACWarlockChar* character = Cast< ACWarlockChar >(oldSkillSender->GetOwner());
	FVector senderLocation = character->GetActorLocation();
	FVector thisLocation = GetActorLocation();
	FVector direction = (senderLocation - thisLocation).GetSafeNormal2D();
	FRotator newRotation = direction.ToOrientationRotator();

	this->SetActorRotation(newRotation);

	projectileMovement->SetVelocityInLocalSpace(FVector(1000.0f, 0.0f, 0.0f));
	return true;

}

UCSkillBase* AActorSpawnedBySkill::SkillThatSpawnedThatActor() const
{
	return _skillThatSpawnedThatActor;
}

void AActorSpawnedBySkill::SetSkillThatSpawnedThatActor(UCSkillBase* val)
{
	_skillThatSpawnedThatActor = val;
}

