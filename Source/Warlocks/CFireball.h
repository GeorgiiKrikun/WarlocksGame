// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSkillBase.h"
#include "CFireball.generated.h"

class ACFireballActorServer;

/**
 * 
 */
UCLASS()
class WARLOCKS_API UCFireball : public UCSkillBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACFireballActorServer> _fireball;


	void ServerSkillCast_Implementation(FVector location) override;

	void ServerAfterSkillCasted_Implementation(FVector location) override;

	void ServerCastDiamondUpgrade_Implementation(FVector location) override;
	
	void ServerAfterCastDiamondUpgrade_Implementation(FVector location) override;

	void onLevelUp() override;

	int getRequiredInputType() override;


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _velocity = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _velocityUpgradeFactor = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _flightTime = 5.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _flightTimeUpgradeFactor = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _damage = 10.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _sizeAndDamageUpgradeFactor = 1.5f;

	ACFireballActorServer* _lastSpawnedFireball = nullptr;

};
