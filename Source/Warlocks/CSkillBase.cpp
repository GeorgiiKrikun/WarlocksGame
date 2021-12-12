// Fill out your copyright notice in the Description page of Project Settings.


#include "CSkillBase.h"
#include "Net/UnrealNetwork.h"
#include "CWarlockMainPlayerController.h"
#include "GoglikeLogging.h"


// Sets default values for this component's properties
UCSkillBase::UCSkillBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;



}



void UCSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCSkillBase, _cooldown);
	DOREPLIFETIME(UCSkillBase, _currentCooldown);
}


void UCSkillBase::ServerSkillCast_Implementation(FVector location /*= FVector()*/)
{
	// do nothing club
}

void UCSkillBase::BeginPreparation_Implementation()
{

}

ACWarlockMainPlayerController* UCSkillBase::getControllerThatPossessThisSkill()
{
	APawn* owner = Cast<APawn>(this->GetOwner());
	if (!owner) {
		GE("No owner of skill was found");
		return nullptr;
	}
	
	return Cast<ACWarlockMainPlayerController>(owner->GetController());
}

int UCSkillBase::getRequiredInputType()
{
	return 0;
}

void UCSkillBase::levelUp_Implementation()
{
	if (_skillLevel < _maxSkillLevel) ++_skillLevel;
}

bool UCSkillBase::canBeLeveledUp()
{
	return _skillLevel < _maxSkillLevel;
}

// Called when the game starts
void UCSkillBase::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
}


// Called every frame
void UCSkillBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (_currentCooldown > 0.0f) _currentCooldown -= DeltaTime;
}