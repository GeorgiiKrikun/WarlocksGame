// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSkillBase.h"
#include "CTeleport.generated.h"

/**
 * 
 */
UCLASS()
class WARLOCKS_API UCTeleport : public UCSkillBase
{
	GENERATED_BODY()
	
public:
	
	void onBEGINCASTServer_Implementation(FVector location, FVector direction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _addZ = 80;



	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
