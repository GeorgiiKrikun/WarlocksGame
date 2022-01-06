// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillParameters.h"
#include "CSkillBase.generated.h"

class ACWarlockMainPlayerController;

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

	ACWarlockMainPlayerController* getControllerThatPossessThisSkill();


	/**
	* Returns input type required for that skill. 0 = no input, 1 - one-vector input
	*/
	UFUNCTION(BlueprintCallable)
	virtual int getRequiredInputType();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	virtual void levelUp();

	UFUNCTION(BlueprintCallable)
	bool canBeLeveledUp();

	int SkillLevel() const { return _skillLevel; }


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//properites
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _cooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _currentCooldown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int _skillLevel = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int _maxSkillLevel = 3;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated)
	TArray<int> _costToLevelUpAtLevel;



public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};