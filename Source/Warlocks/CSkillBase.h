// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSkillBase.generated.h"


UCLASS(Blueprintable, BlueprintType)
class WARLOCKS_API UCSkillBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCSkillBase();

	/// Called from blueprint on client machine to start cooldown
	UFUNCTION(BlueprintCallable)
		void BeginOnPrepare();

	UFUNCTION(server, reliable)
		virtual void onPrepareServer();

	UFUNCTION(client, reliable)
		void prepareClient();

	UFUNCTION(BlueprintNativeEvent)
		void onPrepareClient();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(server, unreliable, BlueprintCallable)
		void writeToLogServer();


	UFUNCTION(client, unreliable, BlueprintCallable)
		void writeToLogClient();

	//properites
	float _cooldown, _currentCooldown;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};