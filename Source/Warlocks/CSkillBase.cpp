// Fill out your copyright notice in the Description page of Project Settings.


#include "CSkillBase.h"

// Sets default values for this component's properties
UCSkillBase::UCSkillBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCSkillBase::BeginOnPrepare()
{
	onPrepareServer();
}

void UCSkillBase::onPrepareServer_Implementation()
{
	if (_currentCooldown < 0.01) {
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

// Called when the game starts
void UCSkillBase::BeginPlay()
{
	Super::BeginPlay();

	/*FString message = "";
	GetOwner()->HasAuthority() ? message = "Has Authority " : message = "No Authority ";
	APawn* pawn = dynamic_cast<APawn*>(GetOwner());
	if (pawn) {
		AController* controller = pawn->GetController();
		//APlayerController* pc = dynamic_cast<APlayerController*>(controller)
		if (controller) {
			message.Append("IsLocal? ");
			message.AppendInt(controller->IsLocalController());
			message.Append(" ");
		} else{
			message.Append("No controller ");
		}
	}

	message.Append("NetMode ");
	message.AppendInt(int(GetNetMode()));
	message.Append(" ");
	message.Append(GetFullName());
	GEngine->AddOnScreenDebugMessage(-1, 150,FColor::Yellow, message);
	*/

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

	// ...
}

