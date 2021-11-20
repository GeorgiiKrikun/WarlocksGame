// Fill out your copyright notice in the Description page of Project Settings.


#include "CTeleport.h"
#include "CWarlockChar.h"
#include "GoglikeLogging.h"

void UCTeleport::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

int UCTeleport::getRequiredInputType()
{
	return 1;
}

void UCTeleport::ServerSkillCast_Implementation(FVector location /*= FVector()*/)
{
	if (_currentCooldown > 0.0f) return;
	_currentCooldown = _cooldown;
	GL("cooldowns %f %f", _currentCooldown, _cooldown);

	

	auto warlock = Cast<ACWarlockChar>(GetOwner());
	location += FVector(0.0f, 0.0f, _addZ);
	warlock->TeleportTo(location, warlock->GetActorRotation(), false, true);
}
