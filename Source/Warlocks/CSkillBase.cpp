// Fill out your copyright notice in the Description page of Project Settings.


#include "CSkillBase.h"

// Sets default values for this component's properties
UCSkillBase::UCSkillBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

}


void UCSkillBase::BeginOnPrepare()
{
	onPrepareServer();
}

void UCSkillBase::onPrepareServer_Implementation()
{
	if (_currentCooldown < 0.01f) {
		prepareClient();
	}
}

void UCSkillBase::prepareClient_Implementation()
{
	onPrepareClient();
}

void UCSkillBase::onPrepareClient_Implementation()
{
	//DO nothing club
}


void UCSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

}

// Called when the game starts
void UCSkillBase::BeginPlay()
{
	Super::BeginPlay();
}

void UCSkillBase::writeToLogServer_Implementation()
{
	FString message = "Server: ";
	message.AppendInt(int(GetNetMode()));
	message.Append(" ");
	message.Append(GetFullName());
	GEngine->AddOnScreenDebugMessage(-1, 150, FColor::Yellow, message);
}

void UCSkillBase::writeToLogClient_Implementation()
{
	FString message = "Client: ";
	message.AppendInt(int(GetNetMode()));
	message.Append(" ");
	message.Append(GetFullName());
	GEngine->AddOnScreenDebugMessage(-1, 150, FColor::Yellow, message);
}


// Called every frame
void UCSkillBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (_currentCooldown > 0.0f) _currentCooldown -= DeltaTime;
}

void UCSkillBase::BeginOnEndPrepare()
{
	onEndPrepareServer();
}


void UCSkillBase::onEndPrepareServer_Implementation()
{
	endPrepareClient();
}

void UCSkillBase::endPrepareClient_Implementation()
{
	onEndPrepareClient();
}

void UCSkillBase::onEndPrepareClient_Implementation()
{
}

void UCSkillBase::BeginBeginCast()
{
	onBeginCastServer();
}

void UCSkillBase::onBeginCastServer_Implementation()
{
	BeginCastClient();
}

void UCSkillBase::BeginCastClient_Implementation()
{
	onBeginCastClient();
}

void UCSkillBase::onBeginCastClient_Implementation()
{

}

void UCSkillBase::BeginAfterCast()
{
	onAfterCastServer();
}

void UCSkillBase::onAfterCastServer_Implementation()
{
	_currentCooldown = _cooldown;
	AfterCastClient();
}

void UCSkillBase::AfterCastClient_Implementation()
{
	onAfterCastClient();
}

void UCSkillBase::onAfterCastClient_Implementation()
{

}


