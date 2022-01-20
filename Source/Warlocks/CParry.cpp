// Fill out your copyright notice in the Description page of Project Settings.


#include "CParry.h"
#include "CWarlockChar.h"

void UCParry::ServerSkillCast_Implementation(FVector location)
{
	ACWarlockChar * character = Cast<ACWarlockChar>(GetOwner());
	if (!character) return;

	character->setInvulnerable(_duration);
}
