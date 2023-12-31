// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillParameters.h"
#include "CSkillBase.generated.h"

class ACWarlockMainPlayerController;
class UTexture2D;

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

	FVector _savedLocation;

	UFUNCTION(Server,Reliable)
	virtual void ServerAfterSkillCasted(FVector location);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ServerCastDiamondUpgrade(FVector location);

	UFUNCTION(Server, Reliable)
	virtual void ServerAfterCastDiamondUpgrade(FVector location);

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void BroadcastAnimationSkillCast();

	UFUNCTION(BlueprintNativeEvent)
	void AnimationSkillCast();

	ACWarlockMainPlayerController* getControllerThatPossessThisSkill();


	/**
	* Returns input type required for that skill. 0 = no input, 1 - one-vector input
	*/
	UFUNCTION(BlueprintCallable)
	virtual int getRequiredInputType();

	UFUNCTION(BlueprintCallable)
	virtual int getRequiredInputTypeDiamond();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void levelUp();

	virtual void onLevelUp();

	UFUNCTION(BlueprintCallable)
	bool canBeLeveledUp();

	UFUNCTION(BlueprintCallable)
	bool canBeDiamondUpgraded();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void diamondUpgrade();

	int SkillLevel() const { return _skillLevel; }

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnLevelChange"))
	FOnSkillLevelChanged onSkillLevelChanged;

	UFUNCTION()
	void OnRep_SkillLevel();

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetSkillIcon() const;

	virtual FText GetSkillDescription() const;
	float CurrentCastTime() const { return _currentCastTime; }
	float CastTime() const { return _castTime; }

	float CurrentCooldown() const { return _currentCooldown; }
	void SetCurrentCooldown(float val) { _currentCooldown = val; }
	bool HasDiamondUpgrade() const { return _hasDiamondUpgrade; }
	void SetHasDiamondUpgrade(bool val) { _hasDiamondUpgrade = val; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//properites
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _cooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _currentCooldown;

	//properites
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _cooldownDiamond;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _currentCooldownDiamond;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _castTime = 0.0f; // by default, immediate
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _castTimeDiamond = 0.0f; // by default, immediate
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _currentCastTime = 0.0f;

	UFUNCTION(NetMulticast, Reliable)
	void startCastTime();

	/**
	 * 0 = Exists as a component in the player but cannot be used yet
	 * 1,2,3... Actual levels
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = OnRep_SkillLevel)
	int _skillLevel = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	int _maxSkillLevel = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool _hasDiamondUpgrade = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool _diamondCastPerformed = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FText> _detailedDesctiption;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<int> _costToLevelUpAtLevel;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int _costToDiamondUpgrade = 3;



public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// UI PART
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* _skillIcon = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText _skillDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool _isSelectedToCast = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	    bool _isSkillSelectedInSkillList = false;

};