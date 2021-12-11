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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _castDelay;

	void ServerSkillCast_Implementation(FVector location) override;


	int getRequiredInputType() override;

};
