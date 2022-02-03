// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSkillBase.h"
#include "SacrificeBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WARLOCKS_API USacrificeBase : public UCSkillBase
{
	GENERATED_BODY()

public:
	USacrificeBase();

	void ServerSkillCast_Implementation(FVector location) override;


	int getRequiredInputType() override;


	void ServerAfterSkillCasted_Implementation(FVector location) override;

};
