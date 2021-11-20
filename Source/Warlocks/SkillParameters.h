// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Union.h" 
#include "Containers/Array.h" 
#include "UObject/NoExportTypes.h"
#include "SkillParameters.generated.h"

/**
 * 
 */

typedef TUnion<FVector> skillParametersUnion;

UCLASS()
class WARLOCKS_API USkillParameters : public UObject
{
	GENERATED_BODY()

public:

	USkillParameters();

	UFUNCTION(BlueprintCallable)
	USkillParameters* appendVector(FVector vector);
	
	UFUNCTION(BlueprintCallable)
	FVector getVector(int position);

	UFUNCTION(BlueprintCallable)
	static USkillParameters* createSkillParameters();

private:
	TArray<skillParametersUnion> _typeUnion;
	
};
