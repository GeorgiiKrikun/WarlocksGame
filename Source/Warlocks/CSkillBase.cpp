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


void UCSkillBase::BeginPREPARE()
{
	onPREPAREServer();
}

void UCSkillBase::onPREPAREServer_Implementation()
{
	if (_currentCooldown < 0.01f) {
		PREPAREClient();
	}
}

void UCSkillBase::PREPAREClient_Implementation()
{
	onPREPAREClient();
}

void UCSkillBase::onPREPAREClient_Implementation()
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

void UCSkillBase::BeginENDPREPARE()
{
	onENDPREPAREServer();
}


void UCSkillBase::onENDPREPAREServer_Implementation()
{
	ENDPREPAREClient();
}

void UCSkillBase::ENDPREPAREClient_Implementation()
{
	onENDPREPAREClient();
}

void UCSkillBase::onENDPREPAREClient_Implementation()
{
}

void UCSkillBase::BeginBEGINCAST()
{
	onBEGINCASTServer();
}

void UCSkillBase::onBEGINCASTServer_Implementation()
{
	BEGINCASTClient();
}

void UCSkillBase::BEGINCASTClient_Implementation()
{
	onBEGINCASTClient();
}

void UCSkillBase::onBEGINCASTClient_Implementation()
{

}

void UCSkillBase::BeginAFTERCAST()
{
	onAFTERCASTServer();
}

void UCSkillBase::onAFTERCASTServer_Implementation()
{
	_currentCooldown = _cooldown;
	/*ACharacter* casted = Cast<ACharacter>(GetOwner());
	FVector location = casted->GetActorLocation();
	AController* controller = casted->GetController();

	FTimerHandle handle;

	GetWorld()->GetTimerManager().SetTimer(handle, [this, location, controller]() {
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 20.0f, location, 200.0f);
		AFTERCASTClient();
		}, 1.0f, 1);
	*/
}

void UCSkillBase::AFTERCASTClient_Implementation()
{
	onAFTERCASTClient();
}

void UCSkillBase::onAFTERCASTClient_Implementation()
{

}


