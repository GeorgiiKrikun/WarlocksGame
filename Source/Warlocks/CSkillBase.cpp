// Fill out your copyright notice in the Description page of Project Settings.


#include "CSkillBase.h"
#include "Net/UnrealNetwork.h"
#include "CWarlockMainPlayerController.h"
#include "GoglikeLogging.h"
#include "CPlayerState.h"
#include "CWarlockChar.h"


// Sets default values for this component's properties
UCSkillBase::UCSkillBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	_costToLevelUpAtLevel.SetNum(_maxSkillLevel+1);
	for (int i = 0; i <= _maxSkillLevel; ++i) {
		_costToLevelUpAtLevel[i] = 5;
	}

}



void UCSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCSkillBase, _skillLevel);
	DOREPLIFETIME(UCSkillBase, _maxSkillLevel);
	DOREPLIFETIME(UCSkillBase, _cooldown);
	DOREPLIFETIME(UCSkillBase, _currentCooldown);
	DOREPLIFETIME(UCSkillBase, _cooldownDiamond);
	DOREPLIFETIME(UCSkillBase, _currentCooldownDiamond);
}


void UCSkillBase::ServerSkillCast_Implementation(FVector location /*= FVector()*/)
{
	_savedLocation = location;
	BroadcastAnimationSkillCast();
}

void UCSkillBase::ServerAfterSkillCasted_Implementation(FVector location)
{

}

void UCSkillBase::ServerCastDiamondUpgrade_Implementation(FVector location)
{
	_savedLocation = location;
}

void UCSkillBase::ServerAfterCastDiamondUpgrade_Implementation(FVector location)
{

}

void UCSkillBase::BroadcastAnimationSkillCast_Implementation()
{
	GL("BroadcastAnumationskillcast")
	AnimationSkillCast();
}

void UCSkillBase::AnimationSkillCast_Implementation()
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

int UCSkillBase::getRequiredInputTypeDiamond()
{
	return 0;
}

void UCSkillBase::levelUp_Implementation()
{
	if (!canBeLeveledUp()) return;

	int oldlevel = _skillLevel;
	++_skillLevel;
	int newlevel = _skillLevel;

	ACPlayerState* state = getControllerThatPossessThisSkill()->GetPlayerState<ACPlayerState>();
	if (state) state->SetCoins(state->Coins() - _costToLevelUpAtLevel[oldlevel]);

	onSkillLevelChanged.Broadcast(0, 0);
}

bool UCSkillBase::canBeLeveledUp()
{
	if ( _skillLevel >= _maxSkillLevel ) return false;
	if ( getControllerThatPossessThisSkill()->GetPlayerState<ACPlayerState>()->Coins() < _costToLevelUpAtLevel[_skillLevel] ) return false;
	return true;
}

void UCSkillBase::OnRep_SkillLevel()
{
	onSkillLevelChanged.Broadcast(0, 0);
}

UTexture2D* UCSkillBase::GetSkillIcon() const
{

	return _skillIcon;
}

FText UCSkillBase::GetSkillDescription() const
{
	return _skillDescription;
}

// Called when the game starts
void UCSkillBase::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
}


void UCSkillBase::startCastTime_Implementation()
{
	_currentCastTime = CastTime();
	ACWarlockChar* warlock = Cast<ACWarlockChar>(GetOwner());
	if (!warlock) return;
	warlock->setCasting(this);
}

// Called every frame
void UCSkillBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentCooldown() > 0.0f) SetCurrentCooldown(CurrentCooldown() - DeltaTime);
	if (CurrentCastTime() > 0.0f) {
		_currentCastTime = CurrentCastTime() - DeltaTime;
		if (CurrentCastTime() <= 0.0f && GetOwner()->HasAuthority()) ServerAfterSkillCasted(_savedLocation);
	}
}