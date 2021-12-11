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


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACMineActorServer> _mine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool _visibleOnThisInstance;


	void ServerSkillCast_Implementation(FVector location) override;

	int getRequiredInputType() override;

};
