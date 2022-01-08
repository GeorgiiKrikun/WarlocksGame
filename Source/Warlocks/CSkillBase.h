// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillParameters.h"
#include "CSkillBase.generated.h"

class ACWarlockMainPlayerController;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWarpEventSignature);

UCLASS(Blueprintable, BlueprintType)
class WARLOCKS_API UCSkillBase : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillLevelChanged, int, oldLevel, int, newLevel);


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

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnLevelChange"))
	FOnSkillLevelChanged onSkillLevelChanged;

	UFUNCTION()
	void OnRep_SkillLevel();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//properites
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _cooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _currentCooldown;

	/**
	 * 0 = Exists as a component in the player but cannot be used yet
	 * 1,2,3... Actual levels
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = OnRep_SkillLevel)
	int _skillLevel = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int _maxSkillLevel = 3;



	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated)
	TArray<int> _costToLevelUpAtLevel;



public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};