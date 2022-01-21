// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorSpawnedBySkill.generated.h"

class UCSkillBase;
class UMovementComponent;

UCLASS()
class WARLOCKS_API AActorSpawnedBySkill : public AActor 
{ 
	GENERATED_BODY() 
	
public:	
	// Sets default values for this actor's properties 
	AActorSpawnedBySkill(); 

protected: 
	// Called when the game starts or when spawned 
	virtual void BeginPlay() override; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UMovementComponent* ProjectileMovement() const;

	/*
	* Return true if it was able to send actor back to sender; false otherwise
	*/
	virtual bool sendBackToSender(UCSkillBase* newSkillSender = nullptr);


protected:
	UCSkillBase* _skillThatSpawnedThatActor;

public:
	UCSkillBase* SkillThatSpawnedThatActor() const;
	void SetSkillThatSpawnedThatActor(UCSkillBase* val);
};
