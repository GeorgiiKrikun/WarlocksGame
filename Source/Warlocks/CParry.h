// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSkillBase.h"
#include "CParry.generated.h"

/**
 * 
 */
UCLASS()
class WARLOCKS_API UCParry : public UCSkillBase
{
	GENERATED_BODY()

public:
	void ServerSkillCast_Implementation(FVector location) override;


	void ServerAfterSkillCasted_Implementation(FVector location) override;

};
