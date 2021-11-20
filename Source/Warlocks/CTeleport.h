// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSkillBase.h"
#include "SkillParameters.h"
#include "CTeleport.generated.h"

/**
 * 
 */
UCLASS()
class WARLOCKS_API UCTeleport : public UCSkillBase
{
	GENERATED_BODY()
	
public:

	UCTeleport();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float _addZ = 80;

	

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ServerSkillCast_Implementation(FVector location = FVector()) override;


	int getRequiredInputType() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _delayBefore;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float _delayAfter;


	UFUNCTION(Client, Reliable)
	void callClientBeforeCast(FVector location);

	UFUNCTION(BlueprintNativeEvent)
	void BeforeCast(FVector location);


	UFUNCTION(Client, Reliable)
	void callClientAfterCast();

	UFUNCTION(BlueprintNativeEvent)
	void AfterCast();

};
