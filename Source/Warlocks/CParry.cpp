// Fill out your copyright notice in the Description page of Project Settings.


#include "CParry.h"
#include "CWarlockChar.h"
#include "GoglikeLogging.h"

void UCParry::ServerSkillCast_Implementation(FVector location)
{
	GL("PARRY CASTED");
	Super::ServerSkillCast_Implementation(location);
	startCastTime();
	ACWarlockChar * character = Cast<ACWarlockChar>(GetOwner());
	if (!character) return;

	character->setInvulnerable(true);
}

void UCParry::ServerAfterSkillCasted_Implementation(FVector location)
{
	ACWarlockChar* character = Cast<ACWarlockChar>(GetOwner());
	if (!character) return;

	character->setInvulnerable(false);
}
