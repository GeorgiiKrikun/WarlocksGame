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

	UFUNCTION(NetMulticast, Reliable)
	void SpawnFireball(FVector location);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACFireballActorServer> _fireball;

	UFUNCTION(NetMulticast, Reliable)
	void DestroyAllMines(int32 num);

	TMap<int32, ACFireballActorServer*> _spawnedFireballs;
	int32 _currentActorSpawnedNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _castDelay;

	void ServerSkillCast_Implementation(FVector location) override;


	int getRequiredInputType() override;

};
