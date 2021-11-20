// Fill out your copyright notice in the Description page of Project Settings.


#include "CTeleport.h"
#include "CWarlockChar.h"
#include "GoglikeLogging.h"

UCTeleport::UCTeleport() : UCSkillBase(),
	_delayBefore(0.5f),
	_delayAfter(0.5f)
{

}

void UCTeleport::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

int UCTeleport::getRequiredInputType()
{
	return 1;
}

void UCTeleport::callClientBeforeCast_Implementation(FVector location)
{
	BeforeCast(location);
}

void UCTeleport::BeforeCast_Implementation(FVector location)
{

}

void UCTeleport::callClientAfterCast_Implementation()
{
	AfterCast();
}

void UCTeleport::AfterCast_Implementation()
{

}

void UCTeleport::ServerSkillCast_Implementation(FVector location /*= FVector()*/)
{
	if (_currentCooldown > 0.0f) return;
	_currentCooldown = _cooldown;
	GL("cooldowns %f %f", _currentCooldown, _cooldown);

	callClientBeforeCast(location);

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this, location]() {
		auto warlock = Cast<ACWarlockChar>(GetOwner());
		FVector newLocation = location + FVector(0.0f, 0.0f, _addZ);
		warlock->TeleportTo(newLocation, warlock->GetActorRotation(), false, true);
		callClientAfterCast();
	}, _delayBefore, false);
}
