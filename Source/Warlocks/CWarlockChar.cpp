// Fill out your copyright notice in the Description page of Project Settings.


#include "CWarlockChar.h"

// Sets default values
ACWarlockChar::ACWarlockChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACWarlockChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACWarlockChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACWarlockChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

