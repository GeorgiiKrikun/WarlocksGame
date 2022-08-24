// Fill out your copyright notice in the Description page of Project Settings.


#include "CAIController.h"

ACAIController::ACAIController(const FObjectInitializer& ObjectInitializer) :
	AAIController(ObjectInitializer)
{
	bWantsPlayerState = true;
}
