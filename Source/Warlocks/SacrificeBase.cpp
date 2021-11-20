// Fill out your copyright notice in the Description page of Project Settings.


#include "SacrificeBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h" 

// Sets default values for this component's properties
USacrificeBase::USacrificeBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicated(true);

}

void USacrificeBase::callClientStopMovement_Implementation()
{
	stopMovement();
}



void USacrificeBase::stopMovement_Implementation()
{

}

int USacrificeBase::getRequiredInputType()
{
	return 0;
}

void USacrificeBase::ServerSkillCast_Implementation(FVector location)
{
	if (_currentCooldown > 0.0f) return;
	_currentCooldown = _cooldown;
	ACharacter* casted = Cast<ACharacter>(GetOwner());
	location = casted->GetActorLocation();
	AController* controller = casted->GetController();

	callClientStopMovement();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this, location, controller]() {
		TArray<AActor*> ingnoredActors;
		ingnoredActors.Add(GetOwner());
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 20.0f, location, 200.0f, 0, ingnoredActors);
		}, 1.0f, false);
}
