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

	UCMine();

	void onBEGINCASTServer_Implementation(FVector location, FVector direction) override;

	UFUNCTION(NetMulticast,Reliable)
	void SpawnMine(FVector location, FVector direction);

	UFUNCTION(NetMulticast, Reliable)
	void DestroyAllMines(int32 num);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACMineActorServer> _mine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool _visibleOnThisInstance;

	TMap<int32, ACMineActorServer*> _spawnedMines;
	int32 _currentActorSpawnedNumber;

};
