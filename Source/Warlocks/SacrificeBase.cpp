// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h" 
#include "CWarlockMainPlayerController.h"

// Sets default values for this component's properties
USacrificeBase::USacrificeBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

int USacrificeBase::getRequiredInputType()
{
	return 0;
}

void USacrificeBase::ServerAfterSkillCasted_Implementation(FVector location)
{
	ACharacter* warlock = Cast<ACharacter>(GetOwner());
	location = warlock->GetActorLocation();
	AController* controller = warlock->GetController();
	TArray<AActor*> ignoredActors;
	ignoredActors.Add(GetOwner());
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 20.0f, location, 200.0f, 0, ignoredActors, GetOwner(), getControllerThatPossessThisSkill());
}

void USacrificeBase::ServerSkillCast_Implementation(FVector location)
{
	if (CurrentCooldown() > 0.0f) return;
	SetCurrentCooldown(_cooldown);
	Super::ServerSkillCast_Implementation(location);
	startCastTime();

}
