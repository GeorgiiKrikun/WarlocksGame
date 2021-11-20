// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillParameters.h"
#include "CSkillBase.generated.h"

UCLASS(Blueprintable, BlueprintType)
class WARLOCKS_API UCSkillBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCSkillBase();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ServerSkillCast(FVector location);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginPreparation();


	/**
	* Returns input type required for that skill. 0 = no input, 1 - one-vector input
	*/
	UFUNCTION(BlueprintCallable)
	virtual int getRequiredInputType();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//properites
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _cooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _currentCooldown;



public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};