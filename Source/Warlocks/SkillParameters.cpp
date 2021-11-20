// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillParameters.h"

USkillParameters::USkillParameters() : UObject()
{
	
}

USkillParameters* USkillParameters::appendVector(FVector vector)
{
	_typeUnion.Add(skillParametersUnion(vector));
	return this;
}

FVector USkillParameters::getVector(int position)
{
	if (_typeUnion.Num() < position) return FVector();
	skillParametersUnion element = _typeUnion[position];
	return element.GetSubtype<FVector>();
}

USkillParameters* USkillParameters::createSkillParameters()
{
	return NewObject<USkillParameters>();
}

