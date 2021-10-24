// Fill out your copyright notice in the Description page of Project Settings.


#include "CTeleport.h"
#include "CWarlockChar.h"

void UCTeleport::onBEGINCASTServer_Implementation(FVector location, FVector direction)
{
	auto warlock = Cast<ACWarlockChar>(GetOwner());
	
	location += FVector(0.0f, 0.0f, _addZ);
	warlock->TeleportTo(location, warlock->GetActorRotation(), false, true);
	BeginAFTERCAST();
}

void UCTeleport::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
