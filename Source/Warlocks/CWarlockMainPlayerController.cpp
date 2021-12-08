// Fill out your copyright notice in the Description page of Project Settings.


#include "CWarlockMainPlayerController.h"
#include "GoglikeLogging.h"
#include "CWarlockChar.h"

void ACWarlockMainPlayerController::callOnPawnRestartClient_Implementation()
{
	GL("callOnPawnRestartClient_Implementation");
	OnPawnRestartClient();
}

void ACWarlockMainPlayerController::OnPawnRestartClient_Implementation()
{
	
}

void ACWarlockMainPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	ACWarlockChar* character = Cast<ACWarlockChar>(aPawn);
	if (!character) return;

	character->onDeathDelegate.AddDynamic(this, &ACWarlockMainPlayerController::reactOnPawnDeath);
}

void ACWarlockMainPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	if (HasAuthority()) {
		GL("OnUnPossess");
		OnPawnDeath.Broadcast();
	}
}

void ACWarlockMainPlayerController::reactOnPawnDeath()
{
	GL("REACT ON PAWN DEATH");
	APawn* pawn = this->GetPawn();
	if (!pawn) return;

	UnPossess();
	pawn->Destroy();
}
