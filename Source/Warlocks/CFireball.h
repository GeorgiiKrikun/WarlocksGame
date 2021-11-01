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

	void onBEGINCASTServer_Implementation(FVector location, FVector direction) override;

	UFUNCTION(NetMulticast, Reliable)
	void SpawnFireball(FVector location, FVector Direction);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACFireballActorServer> _fireball;


};
