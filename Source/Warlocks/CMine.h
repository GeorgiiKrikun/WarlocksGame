// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSkillBase.h"
#include "CMine.generated.h"

class ACMineActorServer;

/**
 * 
 */
UCLASS()
class WARLOCKS_API UCMine : public UCSkillBase
{
	GENERATED_BODY()
	
public:

	void onBEGINCASTServer_Implementation(FVector location, FVector direction) override;

	UFUNCTION(NetMulticast)
	void SpawnMine(FVector location, FVector direction);

	UFUNCTION(NetMulticast)
	void DestroyAllMines();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACMineActorServer> _mine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACMineActorServer _spawnedMine;

};
